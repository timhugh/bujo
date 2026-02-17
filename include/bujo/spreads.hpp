#pragma once

#include "bujo/config.hpp"
#include <date/date.h>
#include <filesystem>
#include <optional>

namespace bujo::spreads {

std::filesystem::path spread_for_date(const config::journal_config &,
                                      const config::spread_config &,
                                      ::date::local_days,
                                      bool render_template = false);

std::filesystem::path current_spread(const config::journal_config &,
                                     const config::spread_config &,
                                     bool render_template = false);

std::filesystem::path
next_spread(const config::journal_config &, const config::spread_config &,
            const std::optional<std::filesystem::path> &current,
            bool render_template = false);

std::filesystem::path
previous_spread(const config::journal_config &, const config::spread_config &,
                const std::optional<std::filesystem::path> &current,
                bool render_template = false);

} // namespace bujo::spreads
