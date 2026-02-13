#pragma once

#include "bujo/config.hpp"
#include "bujo/date.hpp"
#include "bujo/path.hpp"
#include "bujo/span.hpp"
#include "bujo/templates.hpp"
#include <chrono>
#include <date/date.h>
#include <filesystem>
#include <format>
#include <optional>
#include <stdexcept>
#include <system_error>

namespace bujo::spreads {

inline std::filesystem::path
spread_for_date(const config::journal_config &cfg,
                const config::spread_config &spread_cfg,
                ::date::local_days date) {
  auto span = span::Span::from_template(spread_cfg.filename_template, date);
  auto filename =
      templates::render_filename_template(spread_cfg.filename_template, span);
  return config::spread_dir(cfg, spread_cfg) / filename;
}

inline std::filesystem::path
current_spread(const config::journal_config &cfg,
               const config::spread_config &spread_cfg) {
  ::date::local_days date{::date::year_month_day{
      ::date::floor<::date::days>(std::chrono::system_clock::now())}};
  return spread_for_date(cfg, spread_cfg, date);
}

inline span::Span
reference_span(const config::journal_config &cfg,
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

inline std::filesystem::path
next_spread(const config::journal_config &cfg,
            const config::spread_config &spread_cfg,
            const std::optional<std::filesystem::path> &current) {

  auto current_span = reference_span(cfg, spread_cfg, current);
  auto next_date = *current_span.end() + ::date::days{1};
  return spread_for_date(cfg, spread_cfg, next_date);
}

inline std::filesystem::path
previous_spread(const config::journal_config &cfg,
                const config::spread_config &spread_cfg,
                const std::optional<std::filesystem::path> &current) {

  auto current_span = reference_span(cfg, spread_cfg, current);
  auto next_date = *current_span.start() - ::date::days{1};
  return spread_for_date(cfg, spread_cfg, next_date);
}

} // namespace bujo::spreads
