#pragma once

#include "bujo/span.hpp"
#include <date/date.h>
#include <fmt/format.h>
#include <inja/environment.hpp>
#include <inja/function_storage.hpp>
#include <inja/inja.hpp>
#include <sstream>
#include <string>

namespace bujo::templates {

namespace {

inline inja::json to_json(const span::Span &span) {
  std::vector<inja::json> days;
  for (auto day : span.days()) {
    days.push_back({
        {"year", day.format("%Y")},    //
        {"month", day.format("%m")},   //
        {"day", day.format("%d")},     //
        {"weekday", day.format("%A")}, //
        {"iso", day.format("%F")}      //
    });
  }
  return {{"days", days}};
}

inline std::string date_format(const std::string &iso_date,
                               const std::string &format) {
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
  return date::format(format, ymd);
}

} // namespace

inline std::string render_filename_template(const std::string &tmpl,
                                            const span::Span &span) {
  return span.start().format(tmpl);
}

inline const date::local_days
parse_filename_template(const std::string &tmpl, const std::string &filename) {
  std::istringstream in(filename);
  date::local_days date;
  in >> date::parse(tmpl, date);
  if (!in) {
    throw std::invalid_argument(fmt::format(
        "Filename '{}' does not match template '{}'", filename, tmpl));
  }
  in >> std::ws;
  if (!in.eof()) {
    throw std::invalid_argument(
        fmt::format("Filename '{}' has trailing characters after parsing with "
                    "template '{}'",
                    filename, tmpl));
  }
  return date;
}

inline const std::string render_file_template(const std::string &tmpl,
                                              const span::Span &span) {
  inja::Environment env;

  env.add_callback("date_format", [](const inja::Arguments &args) {
    return date_format(args.at(0)->get<std::string>(),
                       args.at(1)->get<std::string>());
  });

  return env.render(tmpl, {{"span", to_json(span)}});
}

} // namespace bujo::templates
