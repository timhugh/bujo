#pragma once

#include <filesystem>
#include <fmt/format.h>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <stdexcept>
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
  std::optional<std::string> commit_message_template;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(git_config,
                                              commit_message_template);
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

journal_config load_from_file(std::filesystem::path);

std::filesystem::path journal_dir(const journal_config &);

std::filesystem::path templates_dir(const journal_config &);

std::filesystem::path notes_dir(const journal_config &);

std::filesystem::path note_dir(const journal_config &, const note_config &);

std::filesystem::path spreads_dir(const journal_config &);

std::filesystem::path spread_dir(const journal_config &, const spread_config &);

note_config note_config_for_key(const journal_config &, const std::string &);

spread_config default_spread_config(const journal_config &);

spread_config spread_config_for_key(const journal_config &,
                                    const std::string &);

} // namespace bujo::config
