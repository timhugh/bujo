#pragma once

#include <date/date.h>
#include <fmt/format.h>
#include <regex>
#include <stdexcept>
#include <string>

namespace bujo::date {

inline std::string format(const std::string &iso_date, const std::string &fmt) {
  std::istringstream in(iso_date);
  ::date::year_month_day ymd;
  in >> ::date::parse("%F", ymd);
  if (!in) {
    throw std::invalid_argument(fmt::format("Invalid ISO date: {}", iso_date));
  }
  in >> std::ws;
  if (!in.eof()) {
    throw std::invalid_argument(
        fmt::format("Invalid ISO date (trailing): {}", iso_date));
  }
  if (!ymd.ok()) {
    throw std::invalid_argument(
        fmt::format("Invalid date components in ISO date: {}", iso_date));
  }
  return ::date::format(fmt, ymd);
}

enum class field { year_Y, iso_year_G, month_m, day_d, iso_week_V, week_W };

struct compiled_template {
  std::regex re;
  std::vector<field> captures;
};

struct capture_map {
  std::optional<int> year;
  std::optional<int> iso_year;
  std::optional<unsigned> month;
  std::optional<unsigned> day;
  std::optional<unsigned> iso_week;
  std::optional<unsigned> week;

  void set(field f, int val) {
    switch (f) {
    case field::year_Y:
      year = val;
      break;
    case field::iso_year_G:
      iso_year = val;
      break;
    case field::month_m:
      month = val;
      break;
    case field::day_d:
      day = val;
      break;
    case field::iso_week_V:
      iso_week = val;
      break;
    case field::week_W:
      week = val;
      break;
    }
  }
};

inline const std::vector<char> regex_metacharacters = {
    '.', '^', '$', '|', '(', ')', '[', ']', '{', '}', '*', '+', '?', '\\'};

inline void append_escaped_literal(std::string &out, char c) {
  if (std::ranges::contains(regex_metacharacters, c)) {
    out.push_back('\\');
  }
  out.push_back(c);
}

inline void append_capture(std::string &out, std::string_view pattern) {
  out.append("(");
  out.append(pattern);
  out.append(")");
}

inline compiled_template compile(std::string_view fmt) {
  std::string pattern;
  pattern.push_back('^');

  compiled_template ct;

  for (size_t i = 0; i < fmt.size(); i++) {
    char c = fmt[i];
    if (c != '%') {
      append_escaped_literal(pattern, c);
      continue;
    }

    if (i + 1 >= fmt.size()) {
      throw std::invalid_argument(
          fmt::format("Stray % at end of template: {}", fmt));
    }

    char d = fmt[++i];
    switch (d) {
      // years
    case 'Y':
      append_capture(pattern, R"(\d{4})");
      ct.captures.push_back(field::year_Y);
      break;

    case 'G':
      append_capture(pattern, R"(\d{4})");
      ct.captures.push_back(field::iso_year_G);
      break;

      // months
    case 'm':
      append_capture(pattern, R"(\d{2})");
      ct.captures.push_back(field::month_m);
      break;

      // TODO: %B
      // TODO: %b / %h

      // days
    case 'd':
      append_capture(pattern, R"(\d{2})");
      ct.captures.push_back(field::day_d);
      break;

    case 'e':
      append_capture(pattern, R"(\d{1,2})");
      ct.captures.push_back(field::day_d);
      break;

      // TODO: %A
      // TODO: %a

      // weeks
    case 'V':
      append_capture(pattern, R"(\d{2})");
      ct.captures.push_back(field::iso_week_V);
      break;

      // composites
    case 'F':
      append_capture(pattern, R"(\d{4})");
      ct.captures.push_back(field::year_Y);
      append_escaped_literal(pattern, '-');

      append_capture(pattern, R"(\d{2})");
      ct.captures.push_back(field::month_m);
      append_escaped_literal(pattern, '-');

      append_capture(pattern, R"(\d{2})");
      ct.captures.push_back(field::day_d);
      break;

    case 'D':
    case 'x':
      // TODO: this needs to be localized
      append_capture(pattern, R"(\d{2})");
      ct.captures.push_back(field::month_m);
      append_escaped_literal(pattern, '/');

      append_capture(pattern, R"(\d{2})");
      ct.captures.push_back(field::day_d);
      append_escaped_literal(pattern, '/');

      append_capture(pattern, R"(\d{4})");
      ct.captures.push_back(field::year_Y);
      break;

      // TODO: %v

      // anything else isn't supported yet
    default:
      throw std::invalid_argument(fmt::format(
          "Unsupported format specifier %{} in template: {}", d, fmt));
    }
  }

  pattern.push_back('$');

  ct.re = std::regex(pattern);
  return ct;
}

inline capture_map match(const compiled_template &ct,
                         const std::string &input) {
  std::cmatch m;
  if (!std::regex_match(input.c_str(), m, ct.re)) {
    throw std::invalid_argument(
        fmt::format("Input does not match template: {}", input));
  }

  if (m.size() != ct.captures.size() + 1) {
    throw std::logic_error(
        fmt::format("Regex capture count does not match expected: {} vs {}",
                    m.size(), ct.captures.size()));
  }

  capture_map out;
  for (size_t i = 0; i < ct.captures.size(); i++) {
    std::string s = m[i + 1].str();
    int val = std::stoi(s);
    out.set(ct.captures[i], val);
  }
  return out;
}

inline ::date::local_days resolve_day(const capture_map &cm) {
  if (!cm.year) {
    throw std::invalid_argument("Day capture requires year");
  }

  if (!cm.month) {
    throw std::invalid_argument("Day capture requires month");
  }

  if (!cm.day) {
    throw std::invalid_argument("Day capture requires day");
  }

  ::date::year y{*cm.year};
  ::date::month m{*cm.month};
  ::date::day d{*cm.day};
  ::date::year_month_day ymd{y, m, d};

  if (!ymd.ok()) {
    throw std::invalid_argument(fmt::format(
        "Invalid year/month combination: {}-{}", *cm.year, *cm.month));
  }

  return ::date::local_days{ymd};
}

enum class week_start { monday, sunday };

inline ::date::local_days floor_to_week_start(::date::local_days d,
                                              week_start ws) {
  const unsigned cur = ::date::weekday{d}.c_encoding();
  const unsigned desired = (ws == week_start::monday) ? 1u : 0u;
  const unsigned delta = (7u + cur - desired) % 7u;
  return d - ::date::days{delta};
}

inline ::date::local_days resolve_iso_week(const capture_map &cm) {
  if (!cm.iso_year) {
    if (cm.year) {
      throw std::invalid_argument(
          "Weekly templates require ISO year (%G). Using %Y creates ambiguous "
          "spans near the start/end of the year.");
    }
    throw std::invalid_argument("Weekly templates require ISO year (%G).");
  }

  if (cm.month) {
    throw std::invalid_argument(
        "Weekly templates cannot include month specifiers (%M, %B, etc.). "
        "Using month and week in tandem creates ambiguous spans near the "
        "start/end of the month.");
  }

  ::date::year_month_day jan4{::date::year{*cm.iso_year} / ::date::January / 4};
  if (!jan4.ok()) {
    throw std::invalid_argument(
        fmt::format("Invalid ISO year: {}", *cm.iso_year));
  }

  ::date::local_days week1_monday =
      floor_to_week_start(::date::local_days{jan4}, week_start::monday);

  ::date::local_days monday =
      week1_monday + ::date::days{7 * (*cm.iso_week - 1)};

  // NOTE: if/when we support weeks starting on sunday:
  // if (ws == week_start::sunday) {
  // 	return monday - ::date::days{1};
  // }

  return monday;
}

inline ::date::local_days resolve_month(const capture_map &cm) {
  if (!cm.year) {
    throw std::invalid_argument(
        "Monthly templates must include the %Y year specifier");
  }

  ::date::year y{*cm.year};
  ::date::month m{*cm.month};
  ::date::year_month_day ymd{y, m, ::date::day{1}};

  if (!ymd.ok()) {
    throw std::invalid_argument(fmt::format(
        "Invalid year/month combination: {}-{}", *cm.year, *cm.month));
  }

  return ::date::local_days{ymd};
}

inline ::date::local_days resolve_year_start(const capture_map &cm) {
  ::date::year y{*cm.year};
  ::date::year_month_day ymd{y, ::date::month{1}, ::date::day{1}};

  if (!ymd.ok()) {
    throw std::invalid_argument(fmt::format("Invalid year: {}", *cm.year));
  }

  return ::date::local_days{ymd};
}

inline ::date::local_days resolve_iso_year_start(const capture_map &cm) {
  ::date::year_month_day jan4{::date::year{*cm.iso_year} / ::date::January / 4};
  if (!jan4.ok()) {
    throw std::invalid_argument(
        fmt::format("Invalid ISO year: {}", *cm.iso_year));
  }

  ::date::local_days monday =
      floor_to_week_start(::date::local_days{jan4}, week_start::monday);

  // NOTE: if/when we support weeks starting on sunday:
  // if (ws == week_start::sunday) {
  // 	return monday - ::date::days{1};
  // }

  return monday;
}

inline ::date::local_days resolve_span_start(const capture_map &cm) {
  if (cm.day) {
    return resolve_day(cm);
  }

  if (cm.month) {
    return resolve_month(cm);
  }

  if (cm.iso_week) {
    return resolve_iso_week(cm);
  }

  if (cm.year) {
    return resolve_year_start(cm);
  }

  if (cm.iso_year) {
    return resolve_iso_year_start(cm);
  }

  throw std::invalid_argument(
      "Template is missing required fields to resolve a date");
}

inline ::date::local_days parse(const std::string &fmt,
                                const std::string &input) {
  auto ct = compile(fmt);
  auto cm = match(ct, input);
  return resolve_span_start(cm);
}

} // namespace bujo::date
