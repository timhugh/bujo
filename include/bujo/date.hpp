#pragma once

#include <date/date.h>
#include <string>

namespace bujo::date {

using namespace ::date;

class invalid_span_exception : public std::invalid_argument {
public:
  invalid_span_exception()
      : std::invalid_argument("End date must be on or after start date") {}
};

class Day {
public:
  Day(local_days day) : day_(day) {}

  std::string format(const std::string &f) const {
    return ::date::format(f, year_month_day{day_});
  }

  constexpr local_days operator*() const noexcept { return day_; }
  constexpr operator local_days() const noexcept { return day_; }

private:
  local_days day_;
};

class Span {
public:
  Span(local_days start_inclusive, local_days end_inclusive)
      : start_(start_inclusive), end_(end_inclusive) {
    if (end_ < start_) {
      throw invalid_span_exception();
    }
  }

  template <typename Duration> class Range {
  public:
    Range(local_days start, local_days end_inclusive)
        : start_(start), end_(end_inclusive + ::date::days{1}),
          duration_(Duration{1}) {
      if (end_ < start_) {
        throw invalid_span_exception();
      }
    }

    class iterator {
    public:
      explicit iterator(local_days cur, Duration duration)
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
      local_days current_;
      Duration duration_;
    };

    iterator begin() const { return iterator{start_, duration_}; }
    iterator end() const { return iterator{end_, duration_}; }

  private:
    local_days start_;
    local_days end_;
    Duration duration_;
  };

  Range<days> days() const { return Range<::date::days>{start_, end_}; }

private:
  local_days start_;
  local_days end_;
};

} // namespace bujo::date
