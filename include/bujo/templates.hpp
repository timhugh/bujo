#pragma once

#include "bujo/span.hpp"
#include <date/date.h>
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
  return date::format(format, ymd);
}

} // namespace

inline const std::string render(const std::string &tmpl,
                                const span::Span &span) {
  inja::Environment env;

  env.add_callback("date_format", [](const inja::Arguments &args) {
    return date_format(args.at(0)->get<std::string>(),
                       args.at(1)->get<std::string>());
  });

  return env.render(tmpl, {{"span", to_json(span)}});
}

} // namespace bujo::templates
