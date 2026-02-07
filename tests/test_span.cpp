#include "bujo/span.hpp"
#include <date/date.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::bujo;
using namespace ::testing;
using namespace ::date;

class DateTest_Span : public ::testing::Test {
protected:
  // week 6 of 2026
  date::local_days start{2026_y / February / 2_d};
  date::local_days end{2026_y / February / 8_d};

  span::Span span{start, end};
};

TEST_F(DateTest_Span, StartAndEnd) {
  EXPECT_EQ(*span.start(), start);
  EXPECT_EQ(*span.end(), end);
}

TEST_F(DateTest_Span, IterateDays) {
  std::vector<date::local_days> iterated_days;

  for (auto day : span.days()) {
    iterated_days.push_back(day);
  }

  std::vector<date::local_days> expected_days;
  for (unsigned int i = 2; i <= 8; i++) {
    date::local_days date{date::year{2026} / February / date::day{i}};
    expected_days.push_back(date);
  }

  EXPECT_THAT(iterated_days, ElementsAreArray(expected_days));
}

TEST(DateTest_SpanErrors, InvalidSpan) {
  // start is after end
  date::local_days start{2026_y / February / 2_d};
  date::local_days end{2026_y / February / 1_d};

  EXPECT_THROW((span::Span{start, end}), span::invalid_span_exception);
}

TEST(DateTest_Day, StringFormat) {
  date::local_days date{2026_y / February / 2_d};
  span::Day day{date};

  EXPECT_EQ(day.format("%Y-%m-%d"), "2026-02-02");
}
