#include "bujo/config.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fmt/format.h>
#include <string>
#include <sys/_types/_pid_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace bujo::exec {

std::string fzf_select(const std::vector<std::string> &items);

void editor(const config::journal_config &, const std::filesystem::path &);

void git_commit(const config::journal_config &, const std::string &message);

void git_push(const config::journal_config &);

void git_pull(const config::journal_config &);

} // namespace bujo::exec
