#include "bujo/date.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::bujo::date;
using namespace ::testing;

TEST(DateTest_Span, IterateDays) {
  // week 6 of 2026
  local_days start{year{2026} / month{2} / day{2}};
  local_days end{year{2026} / month{2} / day{8}};

  Span span{start, end};

  std::vector<local_days> iterated_days;

  for (auto day : span.days()) {
    iterated_days.push_back(day);
  }

  std::vector<local_days> expected_days;
  for (unsigned int i = 2; i <= 8; i++) {
    local_days date{year{2026} / month{2} / day{i}};
    expected_days.push_back(date);
  }

  EXPECT_THAT(iterated_days, ElementsAreArray(expected_days));
}

TEST(DateTest_Span, InvalidSpan) {
  // start is after end
  local_days start{year{2026} / month{2} / day{2}};
  local_days end{year{2026} / month{2} / day{1}};

  EXPECT_THROW((Span{start, end}), invalid_span_exception);
}

TEST(DateTest_Day, StringFormat) {
  local_days date{year{2026} / month{2} / day{2}};
  Day day{date};

  EXPECT_EQ(day.format("%Y-%m-%d"), "2026-02-02");
}
