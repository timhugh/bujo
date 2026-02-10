#pragma once

#include <cstdlib>
#include <filesystem>
#include <optional>
#include <regex>
#include <string>

namespace bujo::path {

inline std::filesystem::path expand_tilde(const std::filesystem::path &path) {
  if (path.empty()) {
    throw std::invalid_argument("Path cannot be empty");
  }

  std::string path_string(path);

  if (path_string[0] != '~') {
    // nothing to do
    return path;
  }

  const char *home = std::getenv("HOME");
  if (!home) {
    throw std::runtime_error("Environment variable HOME is not set");
  }

  return std::string(home) + path_string.substr(1);
}

inline std::filesystem::path expand_env(const std::filesystem::path &path) {
  std::string result(path);
  static std::regex env("\\$\\{([^}]+)\\}");
  std::smatch match;
  while (std::regex_search(result, match, env)) {
    const char *s = std::getenv(match[1].str().c_str());
    const std::string var(s == nullptr ? "" : s);
    result.replace(match[0].first, match[0].second, var);
  }
  return result;
}

inline std::filesystem::path expand(const std::filesystem::path &path) {
  return expand_tilde(expand_env(path));
}

namespace {

inline void append(std::filesystem::path &out, const std::filesystem::path &p) {
  out /= p;
}

inline void append(std::filesystem::path &out,
                   const std::optional<std::filesystem::path> &p) {
  if (p) {
    out /= *p;
  }
}

} // namespace

template <typename... S>
inline std::filesystem::path join(const S &...paths)
  requires((std::is_convertible_v<S, std::filesystem::path> ||
            std::is_convertible_v<S, std::optional<std::filesystem::path>>) &&
           ...)
{
  std::filesystem::path result;
  (append(result, paths), ...);
  return result;
}

} // namespace bujo::path
