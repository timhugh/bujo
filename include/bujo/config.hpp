#pragma once

#include "bujo/path.hpp"
#include <fmt/format.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace bujo::config {

using json = nlohmann::json;

static const std::filesystem::path default_file_path =
    "~/.config/bujo/config.json";

class missing_file_exception : public std::runtime_error {
public:
  explicit missing_file_exception(const std::string &file_path)
      : std::runtime_error(
            fmt::format("Config file '{}' is missing", file_path)) {}
};

class invalid_file_exception : public std::runtime_error {
public:
  explicit invalid_file_exception(const std::string &file_path,
                                  const std::string &error_message)
      : std::runtime_error(fmt::format("Config file '{}' is invalid: {}",
                                       file_path, error_message)) {}
};

struct note_config {
  std::string key;
  std::optional<std::filesystem::path> directory;
  std::optional<std::string> default_template;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(note_config, key, directory,
                                              default_template);
};

struct spread_config {
  std::string key;
  std::string filename_template;
  std::optional<std::filesystem::path> directory;
  std::optional<std::string> default_template;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(spread_config, key,
                                              filename_template, directory,
                                              default_template);
};

struct git_config {
  bool auto_commit;
  std::optional<std::string> commit_message_template;
  std::optional<int> min_commit_interval_seconds;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(git_config, auto_commit,
                                              commit_message_template,
                                              min_commit_interval_seconds);
};

struct journal_config {
  std::filesystem::path journal_root_dir = "~/bujo";
  std::optional<std::filesystem::path> templates_dir =
      std::make_optional("templates");

  std::optional<std::filesystem::path> notes_dir = std::make_optional("notes");
  std::vector<note_config> notes;

  std::optional<std::filesystem::path> spreads_dir =
      std::make_optional("spreads");
  std::vector<spread_config> spreads;

  git_config git;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(journal_config, journal_root_dir,
                                              templates_dir, notes_dir, notes,
                                              spreads_dir, spreads, git);
};

static inline journal_config load_from_file(std::filesystem::path file_path) {
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

inline std::filesystem::path journal_dir(const journal_config &cfg) {
  return path::expand(cfg.journal_root_dir);
}

inline std::filesystem::path templates_dir(const journal_config &cfg) {
  return path::expand(path::join(journal_dir(cfg), cfg.templates_dir));
}

inline std::filesystem::path notes_dir(const journal_config &cfg) {
  return path::expand(path::join(journal_dir(cfg), cfg.notes_dir));
}

inline std::filesystem::path note_dir(const journal_config &cfg,
                                      const note_config &note_cfg) {
  return path::expand(path::join(journal_dir(cfg), note_cfg.directory));
}

inline std::filesystem::path spreads_dir(const journal_config &cfg) {
  return path::expand(path::join(journal_dir(cfg), cfg.spreads_dir));
}

inline std::filesystem::path spread_dir(const journal_config &cfg,
                                        const spread_config &spread_cfg) {
  return path::expand(path::join(spreads_dir(cfg), spread_cfg.directory));
}

inline note_config note_config_for_key(const journal_config &cfg,
                                       const std::string &key) {
  for (const auto &note : cfg.notes) {
    if (note.key == key) {
      return note;
    }
  }
  throw std::invalid_argument("No note config found with key: " + key);
}

inline spread_config default_spread_config(const journal_config &cfg) {
  if (cfg.spreads.empty()) {
    throw std::invalid_argument("No spread configs defined in journal config");
  }
  return cfg.spreads.front();
}

inline spread_config spread_config_for_key(const journal_config &cfg,
                                           const std::string &key) {
  for (const auto &spread : cfg.spreads) {
    if (spread.key == key) {
      return spread;
    }
  }
  throw std::invalid_argument("No spread config found with key: " + key);
}

} // namespace bujo::config
