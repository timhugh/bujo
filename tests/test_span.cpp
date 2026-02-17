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
  local_days start{2026_y / February / 2_d};
  local_days end{2026_y / February / 8_d};

  span::Span span{start, end};
};

TEST_F(DateTest_Span, StartAndEnd) {
  EXPECT_EQ(*span.start(), start);
  EXPECT_EQ(*span.end(), end);
}

TEST_F(DateTest_Span, IterateDays) {
  std::vector<local_days> iterated_days;

  for (auto day : span.days()) {
    iterated_days.push_back(day);
  }

  std::vector<local_days> expected_days;
  for (unsigned int i = 2; i <= 8; i++) {
    local_days date{year{2026} / February / day{i}};
    expected_days.push_back(date);
  }

  EXPECT_THAT(iterated_days, ElementsAreArray(expected_days));
}

TEST(DateTest_SpanErrors, InvalidSpan) {
  // start is after end
  local_days start{2026_y / February / 2_d};
  local_days end{2026_y / February / 1_d};

  EXPECT_THROW((span::Span{start, end}), span::invalid_span_exception);
}

TEST(DateTest_Day, StringFormat) {
  local_days date{2026_y / February / 2_d};
  span::Day day{date};

  EXPECT_EQ(day.format("%Y-%m-%d"), "2026-02-02");
}

TEST(DateTest_SpanFactory, FromTemplate) {
  local_days reference_date{2026_y / February / 4_d};

  // daily template
  span::Span daily_span = span::Span::from_template("%Y-%m-%d", reference_date);
  EXPECT_EQ(*daily_span.start(), reference_date);
  EXPECT_EQ(*daily_span.end(), reference_date);

  // weekly template
  span::Span weekly_span = span::Span::from_template("%G-%V", reference_date);
  EXPECT_EQ(*weekly_span.start(), local_days{2026_y / February / 2_d});
  EXPECT_EQ(*weekly_span.end(), local_days{2026_y / February / 8_d});

  // monthly template
  span::Span monthly_span = span::Span::from_template("%Y-%m", reference_date);
  EXPECT_EQ(*monthly_span.start(), local_days{2026_y / February / 1_d});
  EXPECT_EQ(*monthly_span.end(), local_days{2026_y / February / 28_d});

  // yearly template
  span::Span yearly_span = span::Span::from_template("%Y", reference_date);
  EXPECT_EQ(*yearly_span.start(), local_days{2026_y / January / 1_d});
  EXPECT_EQ(*yearly_span.end(), local_days{2026_y / December / 31_d});
}
