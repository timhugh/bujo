#include "bujo/spreads.hpp"

#include "bujo/config.hpp"
#include "bujo/date.hpp"
#include "bujo/path.hpp"
#include "bujo/span.hpp"
#include "bujo/templates.hpp"
#include <chrono>
#include <date/date.h>
#include <filesystem>
#include <fmt/format.h>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>

namespace bujo::spreads {

std::filesystem::path spread_for_date(const config::journal_config &cfg,
                                      const config::spread_config &spread_cfg,
                                      ::date::local_days date,
                                      bool render_template) {
  auto span = span::Span::from_template(spread_cfg.filename_template, date);
  auto rendered_spread_path =
      templates::render_filename_template(spread_cfg.filename_template, span);
  auto full_spread_path =
      config::spread_dir(cfg, spread_cfg) / rendered_spread_path;

  if (!spread_cfg.default_template ||
      std::filesystem::exists(full_spread_path)) {
    return full_spread_path;
  }

  auto full_template_path =
      config::templates_dir(cfg) / *spread_cfg.default_template;
  if (!std::filesystem::exists(full_template_path)) {
    // TODO: share the logging/output abstractions from main.cpp
    std::cerr << fmt::format("Unable to execute template '{}'. The template "
                             "file does not exist",
                             full_template_path.string())
              << std::endl;
    return full_spread_path;
  }

  std::ifstream template_file(full_template_path);
  if (!template_file.is_open()) {
    // TODO: share the logging/output abstractions from main.cpp
    std::cerr << fmt::format("Unable to execute template '{}'. Could not "
                             "read the template file.",
                             full_template_path.string())
              << std::endl;
    return full_spread_path;
  }

  std::string raw_template((std::istreambuf_iterator<char>(template_file)),
                           std::istreambuf_iterator<char>());
  template_file.close();

  auto rendered_template = templates::render_file_template(raw_template, span);

  std::ofstream new_spread_file(full_spread_path);
  if (!new_spread_file.is_open()) {
    // TODO: share the logging/output abstractions from main.cpp
    std::cerr << fmt::format("Unable to execute template. Could not write to "
                             "spread file '{}'",
                             full_spread_path.string())
              << std::endl;
    return full_spread_path;
  }

  new_spread_file << rendered_template;
  new_spread_file.close();

  return full_spread_path;
}

std::filesystem::path current_spread(const config::journal_config &cfg,
                                     const config::spread_config &spread_cfg,
                                     bool render_template) {
  ::date::local_days date{::date::year_month_day{
      ::date::floor<::date::days>(std::chrono::system_clock::now())}};
  return spread_for_date(cfg, spread_cfg, date, render_template);
}

span::Span reference_span(const config::journal_config &cfg,
                          const config::spread_config &spread_cfg,
                          const std::optional<std::filesystem::path> &current) {
  if (!current) {
    ::date::local_days now{::date::year_month_day{
        ::date::floor<::date::days>(std::chrono::system_clock::now())}};
    return span::Span::from_template(spread_cfg.filename_template, now);
  }

  std::filesystem::path base_spread_path = config::spread_dir(cfg, spread_cfg);
  auto reference_filename = bujo::path::expand(*current);
  std::error_code ec;
  reference_filename =
      std::filesystem::relative(reference_filename, base_spread_path, ec);
  if (ec) {
    throw std::invalid_argument(std::format(
        "Reference file {} is not in spread directory {}: {}",
        reference_filename.string(), base_spread_path.string(), ec.message()));
  }
  auto reference_date =
      bujo::date::parse(spread_cfg.filename_template, reference_filename);
  return span::Span::from_template(spread_cfg.filename_template,
                                   reference_date);
}

std::filesystem::path next_spread(
    const config::journal_config &cfg, const config::spread_config &spread_cfg,
    const std::optional<std::filesystem::path> &current, bool render_template) {

  auto current_span = reference_span(cfg, spread_cfg, current);
  auto next_date = *current_span.end() + ::date::days{1};
  return spread_for_date(cfg, spread_cfg, next_date, render_template);
}

std::filesystem::path previous_spread(
    const config::journal_config &cfg, const config::spread_config &spread_cfg,
    const std::optional<std::filesystem::path> &current, bool render_template) {

  auto current_span = reference_span(cfg, spread_cfg, current);
  auto next_date = *current_span.start() - ::date::days{1};
  return spread_for_date(cfg, spread_cfg, next_date, render_template);
}

} // namespace bujo::spreads
