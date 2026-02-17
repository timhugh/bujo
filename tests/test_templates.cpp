#include "bujo/templates.hpp"

#include "bujo/span.hpp"
#include <date/date.h>
#include <gtest/gtest.h>
#include <string>

using namespace ::bujo;
using namespace ::date;
using namespace ::testing;

TEST(TemplatesTest_RenderFilenameTemplate, Parses) {
  std::string tmpl = "%Y-%m-%d.txt";
  std::string filename = "2026-02-02.txt";

  EXPECT_EQ(templates::parse_filename_template(tmpl, filename),
            local_days{2026_y / February / 2_d});
}

class TemplatesTest_RenderFileTemplate : public Test {
protected:
  // week 6 of 2026
  local_days start{2026_y / February / 2_d};
  local_days end{2026_y / February / 8_d};

  span::Span span{start, end};
};

TEST_F(TemplatesTest_RenderFileTemplate, RenderWithSpanData) {
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

  EXPECT_EQ(templates::render_file_template(tmpl, span), expected);
}

TEST_F(TemplatesTest_RenderFileTemplate, RenderDateFormatCallback) {
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

  EXPECT_EQ(templates::render_file_template(tmpl, span), expected);
}
