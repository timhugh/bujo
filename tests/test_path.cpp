#include "bujo/path.hpp"

#include <cstdlib>
#include <filesystem>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace ::bujo::path;

class PathTest : public Test {
protected:
  std::filesystem::path home = std::getenv("HOME");
};

TEST_F(PathTest, ExpandTilde) {
  EXPECT_EQ(expand_tilde("~"), home);
  EXPECT_EQ(expand_tilde("~/documents"), home / "documents");
}

TEST_F(PathTest, ExpandEnv) {
  EXPECT_EQ(expand_env("${HOME}"), home);
  EXPECT_EQ(expand_env("${HOME}/documents"), home / "documents");
}

TEST_F(PathTest, ExpandEnvRecursive) {
  setenv("BUJO_ENV_TEST_PATH", "${HOME}/bujo", true);

  EXPECT_EQ(expand_env("${BUJO_ENV_TEST_PATH}"), home / "bujo");
}

TEST_F(PathTest, ExpandAll) {
  setenv("BUJO_ENV_TEST_PATH", "bujo/test", true);

  EXPECT_EQ(expand("~/${BUJO_ENV_TEST_PATH}"), home / "bujo/test");
}

TEST_F(PathTest, ExpandAllWithTildeInVariable) {
  setenv("BUJO_ENV_TEST_PATH", "~/${BUJO_ENV_TEST_PATH_2}", true);
  setenv("BUJO_ENV_TEST_PATH_2", "bujo/test", true);

  EXPECT_EQ(expand("${BUJO_ENV_TEST_PATH}"), home / "bujo/test");
}

TEST(PathJoinTest, JoinsPathsAndOptionalPaths) {
  std::filesystem::path base{"/home/me/journal"};
  std::optional<std::filesystem::path> spreads{"spreads"};
  std::filesystem::path file{"2026/02-04.md"};

  EXPECT_EQ(join(base, spreads, file),
            "/home/me/journal/spreads/2026/02-04.md");
}
