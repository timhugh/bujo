#include "bujo/span.hpp"

#include "bujo/date.hpp"
#include <date/date.h>
#include <stdexcept>
#include <string>

namespace bujo::span {

Span Span::from_template(const std::string &fmt,
                         const ::date::local_days &reference_date,
                         const int max_iterations) {
  const std::string reference_rendered = date::format(reference_date, fmt);
  ::date::local_days start, end;

  std::string current_rendered = reference_rendered;
  ::date::local_days current = reference_date;
  int iterations = 0;
  // iterate backward to find start date
  while (current_rendered == reference_rendered) {
    if (iterations >= max_iterations) {
      throw std::runtime_error(
          "Max iterations reached while searching for start date");
    }
    current = current - ::date::days{1};
    current_rendered = date::format(current, fmt);
  }
  start = current + ::date::days{1};

  current = reference_date;
  current_rendered = reference_rendered;
  iterations = 0;
  // iterate forward to find end date
  while (current_rendered == reference_rendered) {
    if (iterations >= max_iterations) {
      throw std::runtime_error(
          "Max iterations reached while searching for end date");
    }
    current = current + ::date::days{1};
    current_rendered = date::format(current, fmt);
  }
  end = current - ::date::days{1};

  return Span{start, end};
};

} // namespace bujo::span
