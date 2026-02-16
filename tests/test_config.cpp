#include <filesystem>
#include <gtest/gtest.h>

#include "bujo/config.hpp"

using namespace bujo;

std::filesystem::path data = BUJO_TEST_DATA_DIR;

TEST(ConfigTest, LoadFromFile) {
  auto file_path = data / "full_config.json";
  auto c = config::load_from_file(file_path);

  EXPECT_EQ("~/my_journal", c.journal_root_dir);
  EXPECT_EQ("my_templates", c.templates_dir);
  EXPECT_EQ("my_notes", c.notes_dir);

  EXPECT_EQ(1, c.notes.size());
  EXPECT_EQ("daily-sync", c.notes[0].key);
  EXPECT_EQ("daily_sync_notes", c.notes[0].directory);
  EXPECT_EQ("daily_sync_template.md", c.notes[0].default_template);

  EXPECT_EQ("my_spreads", c.spreads_dir);
  EXPECT_EQ(2, c.spreads.size());
  EXPECT_EQ("daily", c.spreads[0].key);
  EXPECT_EQ("daily_spreads", c.spreads[0].directory);
  EXPECT_EQ("%Y-%m-%d.md", c.spreads[0].filename_template);
  EXPECT_EQ("daily_spread_template.md", c.spreads[0].default_template);

  EXPECT_EQ("monthly", c.spreads[1].key);
  EXPECT_EQ("monthly_spreads", c.spreads[1].directory);
  EXPECT_EQ("%Y-%m.md", c.spreads[1].filename_template);
  EXPECT_EQ("monthly_spread_template.md", c.spreads[1].default_template);

  EXPECT_EQ("update %c", c.git.commit_message_template);
}

TEST(ConfigTest, LoadMissingFile) {
  EXPECT_THROW(config::load_from_file(data / "missing_config.json"),
               config::missing_file_exception);
}

TEST(ConfigTest, LoadInvalidFile) {
  EXPECT_THROW(config::load_from_file(data / "invalid_config.json"),
               config::invalid_file_exception);
}
