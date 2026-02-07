#pragma once

#include "bujo/date.hpp"
#include <date/date.h>
#include <string>

namespace bujo::span {

class invalid_span_exception : public std::invalid_argument {
public:
  invalid_span_exception()
      : std::invalid_argument("End date must be on or after start date") {}
};

class Day {
public:
  Day(::date::local_days day) : day_(day) {}

  std::string format(const std::string &f) const {
    return ::date::format(f, ::date::year_month_day{day_});
  }

  constexpr ::date::local_days operator*() const noexcept { return day_; }
  constexpr operator ::date::local_days() const noexcept { return day_; }

private:
  ::date::local_days day_;
};

class Span {
public:
  Span(::date::local_days start_inclusive, ::date::local_days end_inclusive)
      : start_(start_inclusive), end_(end_inclusive) {
    if (end_ < start_) {
      throw invalid_span_exception();
    }
  }

  static Span from_template(const std::string &fmt,
                            const ::date::local_days &reference_date,
                            const int max_iterations = 365) {
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
  }

  template <typename Duration> class Range {
  public:
    Range(::date::local_days start, ::date::local_days end_inclusive)
        : start_(start), end_(end_inclusive + ::date::days{1}),
          duration_(Duration{1}) {
      if (end_ < start_) {
        throw invalid_span_exception();
      }
    }

    class iterator {
    public:
      explicit iterator(::date::local_days cur, Duration duration)
          : current_(cur), duration_(duration) {}

      Day operator*() const { return current_; }

      iterator &operator++() {
        current_ += duration_;
        return *this;
      }

      friend bool operator!=(const iterator &a, const iterator &b) {
        return a.current_ != b.current_;
      }

    private:
      ::date::local_days current_;
      Duration duration_;
    };

    iterator begin() const { return iterator{start_, duration_}; }
    iterator end() const { return iterator{end_, duration_}; }

  private:
    ::date::local_days start_;
    ::date::local_days end_;
    Duration duration_;
  };

  Range<::date::days> days() const { return Range<::date::days>{start_, end_}; }

  Day start() const { return start_; }
  Day end() const { return end_; }

private:
  ::date::local_days start_;
  ::date::local_days end_;
};

} // namespace bujo::span
