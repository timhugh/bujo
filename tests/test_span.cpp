#include "bujo/span.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::bujo;
using namespace ::testing;

TEST(DateTest_Span, IterateDays) {
  // week 6 of 2026
  date::local_days start{date::year{2026} / date::month{2} / date::day{2}};
  date::local_days end{date::year{2026} / date::month{2} / date::day{8}};

  span::Span span{start, end};

  std::vector<date::local_days> iterated_days;

  for (auto day : span.days()) {
    iterated_days.push_back(day);
  }

  std::vector<date::local_days> expected_days;
  for (unsigned int i = 2; i <= 8; i++) {
    date::local_days date{date::year{2026} / date::month{2} / date::day{i}};
    expected_days.push_back(date);
  }

  EXPECT_THAT(iterated_days, ElementsAreArray(expected_days));
}

TEST(DateTest_Span, InvalidSpan) {
  // start is after end
  date::local_days start{date::year{2026} / date::month{2} / date::day{2}};
  date::local_days end{date::year{2026} / date::month{2} / date::day{1}};

  EXPECT_THROW((span::Span{start, end}), span::invalid_span_exception);
}

TEST(DateTest_Day, StringFormat) {
  date::local_days date{date::year{2026} / date::month{2} / date::day{2}};
  span::Day day{date};

  EXPECT_EQ(day.format("%Y-%m-%d"), "2026-02-02");
}
