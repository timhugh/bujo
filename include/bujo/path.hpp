#pragma once

#include <cstdlib>
#include <filesystem>
#include <optional>

namespace bujo::path {

std::filesystem::path expand_tilde(const std::filesystem::path &path);

std::filesystem::path expand_env(const std::filesystem::path &path);

std::filesystem::path expand(const std::filesystem::path &path);

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
