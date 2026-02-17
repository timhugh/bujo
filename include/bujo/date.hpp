#pragma once

#include <cstddef>
#include <date/date.h>
#include <fmt/format.h>
#include <string>

namespace bujo::date {

std::string format(const ::date::local_days &date, const std::string &fmt);

::date::local_days parse(const std::string &fmt, const std::string &input);

} // namespace bujo::date
