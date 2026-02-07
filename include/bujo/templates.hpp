#pragma once

#include "bujo/date.hpp"
#include "bujo/span.hpp"
#include <ctime>
#include <date/date.h>
#include <fmt/format.h>
#include <inja/environment.hpp>
#include <inja/function_storage.hpp>
#include <inja/inja.hpp>
#include <string>

namespace bujo::templates {

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

inline std::string render_filename_template(const std::string &tmpl,
                                            const span::Span &span) {
  return span.start().format(tmpl);
}

inline const ::date::local_days
parse_filename_template(const std::string &tmpl, const std::string &filename) {
  return date::parse(tmpl, filename);
}

inline const std::string render_file_template(const std::string &tmpl,
                                              const span::Span &span) {
  inja::Environment env;

  env.add_callback("date_format", [](const inja::Arguments &args) {
    return date::format(args.at(0)->get<std::string>(),
                        args.at(1)->get<std::string>());
  });

  return env.render(tmpl, {{"span", to_json(span)}});
}

} // namespace bujo::templates
