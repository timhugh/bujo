#include "bujo/exec.hpp"
#include "bujo/config.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fmt/format.h>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace bujo::exec {

const std::string fzf_command = "fzf";
const std::vector<std::string> fzf_flags = {"--reverse"};

std::string fzf_select(const std::vector<std::string> &items) {
  int input_pipe[2];
  int output_pipe[2];

  if (pipe(input_pipe) == -1 || pipe(output_pipe) == -1) {
    throw std::runtime_error("Failed to create pipes for fzf");
  }

  pid_t pid = fork();

  if (pid == -1) {
    throw std::runtime_error("Failed to fork fzf");
  }

  if (pid == 0) {
    dup2(input_pipe[0], STDIN_FILENO);
    close(input_pipe[0]);
    close(input_pipe[1]);

    dup2(output_pipe[1], STDOUT_FILENO);
    close(output_pipe[0]);
    close(output_pipe[1]);

    // TODO: pass all args as variadic
    execlp(fzf_command.c_str(), fzf_command.c_str(), fzf_flags[0].c_str(),
           nullptr);

    // If execlp returns, there was an error
    throw std::runtime_error("Failed to execute fzf");
  }

  close(input_pipe[0]);
  close(output_pipe[1]);

  for (const auto &item : items) {
    std::string line = item + "\n";
    write(input_pipe[1], line.c_str(), line.length());
  }
  close(input_pipe[1]);

  std::string result;
  char buffer[128];
  ssize_t bytes_read;

  while ((bytes_read = read(output_pipe[0], buffer, sizeof(buffer))) > 0) {
    result.append(buffer, bytes_read);
  }
  close(output_pipe[0]);

  int status;
  waitpid(pid, &status, 0);

  if (!result.empty() && result.back() == '\n') {
    result.pop_back();
  }

  return result;
}

void editor(const config::journal_config &cfg,
            const std::filesystem::path &target) {
  std::string editor = std::getenv("EDITOR");
  if (editor.empty()) {
    throw std::runtime_error("EDITOR environment variable is not set");
  }
  std::string command = fmt::format("{} {}", editor, target.string());
  system(command.c_str());
}

void git_commit(const config::journal_config &cfg, const std::string &message) {
  auto base_path = config::journal_dir(cfg);
  system(fmt::format("cd {} && git add .", base_path.string()).c_str());
  system(
      fmt::format("cd {} && git commit -m \"{}\"", base_path.string(), message)
          .c_str());
}

void git_push(const config::journal_config &cfg) {
  auto base_path = config::journal_dir(cfg);
  system(fmt::format("cd {} && git push", base_path.string()).c_str());
}

void git_pull(const config::journal_config &cfg) {
  auto base_path = config::journal_dir(cfg);
  system(fmt::format("cd {} && git pull --rebase", base_path.string()).c_str());
}

} // namespace bujo::exec
