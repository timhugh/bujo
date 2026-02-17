#pragma once

#include "bujo/config.hpp"
#include <filesystem>
#include <vector>

namespace bujo::documents {

std::vector<std::filesystem::path> list_all(const config::journal_config &cfg,
                                            bool relative_paths);

} // namespace bujo::documents
