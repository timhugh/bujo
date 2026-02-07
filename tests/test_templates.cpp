#include <date/date.h>
#include <gtest/gtest.h>

#include "bujo/span.hpp"
#include "bujo/templates.hpp"

using namespace ::bujo;
using namespace ::date;
using namespace ::testing;

struct FilenameTestCase {
  span::Span span;
  std::string template_str;
  std::string expected_filename;
};

void PrintTo(const FilenameTestCase &test_case, std::ostream *os) {
  *os << "span: [" << *test_case.span.start() << ", " << *test_case.span.end()
      << "], template: '" << test_case.template_str;
}

class TemplatesTest_FilenameTemplates : public TestWithParam<FilenameTestCase> {
};

TEST_P(TemplatesTest_FilenameTemplates, RendersExpectedTemplate) {
  auto rendered = templates::render_filename_template(GetParam().template_str,
                                                      GetParam().span);
  EXPECT_EQ(rendered, GetParam().expected_filename);
  auto parsed =
      templates::parse_filename_template(GetParam().template_str, rendered);
  EXPECT_EQ(parsed, *GetParam().span.start());
}

INSTANTIATE_TEST_SUITE_P(
    RendersAndParses, TemplatesTest_FilenameTemplates,
    Values(FilenameTestCase{span::Span{local_days{2026_y / February / 2_d},
                                       local_days{2026_y / February / 8_d}},
                            "%Y-%m-%d.md", "2026-02-02.md"} //
           // FilenameTestCase{span::Span{local_days{2026_y / February / 2_d},
           //                             local_days{2026_y / February / 8_d}},
           //                  "%G-W%W.md", "2026-W05.md"} //
           ));

TEST(TemplatesTest_RenderFilenameTemplate, Parses) {
  std::string tmpl = "week-%Y-%m-%d.txt";
  std::string filename = "week-2026-02-02.txt";

  EXPECT_EQ(templates::parse_filename_template(tmpl, filename),
            local_days{2026_y / February / 2_d});
}

class TemplatesTest_RenderFileTemplate : public Test {
protected:
  // week 6 of 2026
  date::local_days start{date::year{2026} / date::month{2} / date::day{2}};
  date::local_days end{date::year{2026} / date::month{2} / date::day{8}};

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
