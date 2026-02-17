#pragma once

#include "bujo/span.hpp"
#include <ctime>
#include <date/date.h>
#include <fmt/format.h>
#include <inja/environment.hpp>
#include <inja/function_storage.hpp>
#include <inja/inja.hpp>
#include <inja/json.hpp>
#include <string>

namespace bujo::templates {

std::string render_filename_template(const std::string &tmpl,
                                     const span::Span &span);

const ::date::local_days parse_filename_template(const std::string &tmpl,
                                                 const std::string &filename);

const std::string render_file_template(const std::string &tmpl,
                                       const span::Span &span);

} // namespace bujo::templates
