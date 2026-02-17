#include "bujo/templates.hpp"

#include "bujo/date.hpp"
#include "bujo/span.hpp"
#include <ctime>
#include <date/date.h>
#include <fmt/format.h>
#include <inja/environment.hpp>
#include <inja/function_storage.hpp>
#include <inja/inja.hpp>
#include <inja/json.hpp>
#include <string>
#include <vector>

namespace bujo::templates {

inja::json to_json(const span::Span &span) {
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

std::string render_filename_template(const std::string &tmpl,
                                     const span::Span &span) {
  return span.start().format(tmpl);
}

const ::date::local_days parse_filename_template(const std::string &tmpl,
                                                 const std::string &filename) {
  return date::parse(tmpl, filename);
}

const std::string render_file_template(const std::string &tmpl,
                                       const span::Span &span) {
  inja::Environment env;

  env.add_callback("date_format", [](const inja::Arguments &args) {
    const auto &date_string = args.at(0)->get<std::string>();
    const auto &fmt = args.at(1)->get<std::string>();

    ::date::local_days parsed_date = date::parse("%Y-%m-%d", date_string);
    return date::format(parsed_date, fmt);
  });

  return env.render(tmpl, {{"span", to_json(span)}});
}

} // namespace bujo::templates
