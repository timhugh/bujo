#include <gtest/gtest.h>

#include "bujo/span.hpp"
#include "bujo/templates.hpp"

using namespace bujo;

class TemplatesTest_Render : public ::testing::Test {
protected:
  // week 6 of 2026
  date::local_days start{date::year{2026} / date::month{2} / date::day{2}};
  date::local_days end{date::year{2026} / date::month{2} / date::day{8}};

  span::Span span{start, end};
};

TEST_F(TemplatesTest_Render, RenderWithSpanData) {
  std::string tmpl = R"(
  {%- for day in span.days -%}
  {{ day.year }}-{{ day.month }}-{{ day.day }} {{ day.weekday }}
	{%- endfor -%}
  )";

  std::string expected = R"(
2026-02-02 Monday
2026-02-03 Tuesday
2026-02-04 Wednesday
2026-02-05 Thursday
2026-02-06 Friday
2026-02-07 Saturday
2026-02-08 Sunday
)";

  EXPECT_EQ(templates::render(tmpl, span), expected);
}

TEST_F(TemplatesTest_Render, RenderDateFormatCallback) {
  std::string tmpl = R"(
  {%- for day in span.days -%}
	  {{ date_format(day.iso, "%Y-%m-%d %A") }}
	{%- endfor -%}
	)";

  std::string expected = R"(
2026-02-02 Monday
2026-02-03 Tuesday
2026-02-04 Wednesday
2026-02-05 Thursday
2026-02-06 Friday
2026-02-07 Saturday
2026-02-08 Sunday
)";

  EXPECT_EQ(templates::render(tmpl, span), expected);
}
