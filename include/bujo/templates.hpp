#pragma once

#include <inja/environment.hpp>
#include <inja/inja.hpp>
#include <string>

namespace bujo::templates {

inline const std::string render(const std::string &tmpl) {
  return inja::render(tmpl, {});
}

} // namespace bujo::templates
