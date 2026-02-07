#include <gtest/gtest.h>

#include "bujo/date.hpp"
#include "bujo/templates.hpp"

using namespace bujo;
using namespace bujo::date;
using namespace bujo::templates;

class TemplatesTest_Render : public ::testing::Test {
protected:
  // week 6 of 2026
  local_days start{year{2026} / month{2} / day{2}};
  local_days end{year{2026} / month{2} / day{8}};

  Span span{start, end};
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

  EXPECT_EQ(render(tmpl, span), expected);
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

  EXPECT_EQ(render(tmpl, span), expected);
}
