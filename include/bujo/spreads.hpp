#pragma once

#include "bujo/config.hpp"
#include <date/date.h>
#include <filesystem>
#include <optional>

namespace bujo::spreads {

std::filesystem::path spread_for_date(const config::journal_config &,
                                      const config::spread_config &,
                                      ::date::local_days);

std::filesystem::path current_spread(const config::journal_config &,
                                     const config::spread_config &);

std::filesystem::path
next_spread(const config::journal_config &, const config::spread_config &,
            const std::optional<std::filesystem::path> &current);

std::filesystem::path
previous_spread(const config::journal_config &, const config::spread_config &,
                const std::optional<std::filesystem::path> &current);

} // namespace bujo::spreads
