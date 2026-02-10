#pragma once

#include "bujo/config.hpp"
#include <filesystem>
#include <vector>

namespace bujo::documents {

inline std::vector<std::filesystem::path>
list_all(const config::journal_config &cfg, bool relative_paths) {
  auto base_path = config::journal_dir(cfg);
  std::vector<std::filesystem::path> result;
  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(base_path)) {
    if (entry.is_regular_file() && entry.path().extension() == ".md") {
      if (relative_paths) {
        auto relative_path = std::filesystem::relative(entry.path(), base_path);
        result.push_back(relative_path);
      } else {
        result.push_back(entry.path());
      }
    }
  }
  return result;
}

} // namespace bujo::documents
