#include "bujo/config.hpp"

#include "bujo/path.hpp"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

namespace bujo::config {

journal_config load_from_file(std::filesystem::path file_path) {
  std::ifstream f(file_path);
  if (!f.is_open()) {
    throw missing_file_exception(file_path);
  }

  try {
    auto j = json::parse(f);
    return j.get<journal_config>();
  } catch (const json::exception &parse_exception) {
    throw invalid_file_exception(file_path, parse_exception.what());
  }
}

std::filesystem::path journal_dir(const journal_config &cfg) {
  return path::expand(cfg.journal_root_dir);
}

std::filesystem::path templates_dir(const journal_config &cfg) {
  return path::expand(path::join(journal_dir(cfg), cfg.templates_dir));
}

std::filesystem::path notes_dir(const journal_config &cfg) {
  return path::expand(path::join(journal_dir(cfg), cfg.notes_dir));
}

std::filesystem::path note_dir(const journal_config &cfg,
                               const note_config &note_cfg) {
  return path::expand(path::join(journal_dir(cfg), note_cfg.directory));
}

std::filesystem::path spreads_dir(const journal_config &cfg) {
  return path::expand(path::join(journal_dir(cfg), cfg.spreads_dir));
}

std::filesystem::path spread_dir(const journal_config &cfg,
                                 const spread_config &spread_cfg) {
  return path::expand(path::join(spreads_dir(cfg), spread_cfg.directory));
}

note_config note_config_for_key(const journal_config &cfg,
                                const std::string &key) {
  for (const auto &note : cfg.notes) {
    if (note.key == key) {
      return note;
    }
  }
  throw std::invalid_argument("No note config found with key: " + key);
}

spread_config default_spread_config(const journal_config &cfg) {
  if (cfg.spreads.empty()) {
    throw std::invalid_argument("No spread configs defined in journal config");
  }
  return cfg.spreads.front();
}

spread_config spread_config_for_key(const journal_config &cfg,
                                    const std::string &key) {
  for (const auto &spread : cfg.spreads) {
    if (spread.key == key) {
      return spread;
    }
  }
  throw std::invalid_argument("No spread config found with key: " + key);
}
} // namespace bujo::config
