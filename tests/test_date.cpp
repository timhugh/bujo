#include <date/date.h>
#include <gtest/gtest.h>
#include <stdexcept>

#include "bujo/date.hpp"

using namespace ::bujo::date;
using namespace ::date;
using namespace ::testing;

TEST(DateTest, Parse) {
  // minimum unit is day
  local_days date{2026_y / February / 4_d};
  EXPECT_EQ(parse("%Y-%m-%d", "2026-02-04"), date);
  EXPECT_EQ(parse("%Y-%m-%e", "2026-02-4"), date);
  EXPECT_EQ(parse("%F", "2026-02-04"), date);
  EXPECT_EQ(parse("%D", "02/04/2026"), date);
  EXPECT_EQ(parse("%x", "02/04/2026"), date);
  EXPECT_EQ(parse("%Y%m%d", "20260204"), date);

  // minimum unit is month
  EXPECT_EQ(parse("%Y-%m", "2026-02"), local_days{2026_y / February / 1_d});

  // minimum unit is year
  EXPECT_EQ(parse("%Y", "2026"), local_days{2026_y / January / 1_d});

  // ISO weeks and years
  EXPECT_EQ(parse("%G", "2026"), local_days{2025_y / December / 29_d});
  EXPECT_EQ(parse("%G/%V", "2026/06"), local_days{2026_y / February / 2_d});
  EXPECT_EQ(parse("%G/%V", "2026/01"), local_days{2025_y / December / 29_d});
  EXPECT_EQ(parse("%G/%V", "2027/00"), local_days{2026_y / December / 28_d});
  EXPECT_EQ(parse("%G/%V", "2027/01"), local_days{2027_y / January / 4_d});

  // arbitrary positions
  EXPECT_EQ(parse("spreads/%Y/%m-%d.md", "spreads/2026/02-04.md"), date);

  // weird edge cases
  EXPECT_EQ(parse("%Y-%Y", "2026-2026"), local_days{2026_y / January / 1_d});
}

TEST(DateTest, ParseErrors) {
  // literal mismatch
  EXPECT_THROW(parse("%Y-%m-%d.md", "2026-04-12.txt"), std::invalid_argument);

  // invalid format
  EXPECT_THROW(parse("%Y-%m-%", "2026-02"), std::invalid_argument);

  // unsupported specifiers
  EXPECT_THROW(parse("%Y-%W", "2026-02"), std::invalid_argument);

  // invalid input
  EXPECT_THROW(parse("%Y-%m-%d", "2026-02-x"), std::invalid_argument);
  EXPECT_THROW(parse("%Y-%m-%d", "x-02-04"), std::invalid_argument);
  // invalid dates
  EXPECT_THROW(parse("%Y-%m-%d", "2026-13-01"), std::invalid_argument);
  EXPECT_THROW(parse("%Y-%m-%d", "2026-02-90"), std::invalid_argument);

  // year is required
  EXPECT_THROW(parse("%m-%d", "06-05"), std::invalid_argument);
  EXPECT_THROW(parse("%d", "05"), std::invalid_argument);
  EXPECT_THROW(parse("%V", "05"), std::invalid_argument);

  // month + week is invalid
  EXPECT_THROW(parse("%m%V", "0104"), std::invalid_argument);

  // ISO week requires ISO year
  EXPECT_THROW(parse("%Y%V", "202604"), std::invalid_argument);

  // weird edge cases
  // EXPECT_THROW(parse("%Y-%Y", "2026-2027"), std::invalid_argument);
}
