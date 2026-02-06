#include <gtest/gtest.h>

#include "bujo/templates.hpp"

using namespace bujo;

TEST(TemplatesTest, RenderSpanDates) {
  const std::string templ = "";

  auto rendered = templates::render(templ);

  EXPECT_EQ("", rendered);
}
