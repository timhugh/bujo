#include "bujo/config.hpp"
#include "bujo/date.hpp"
#include "bujo/documents.hpp"
#include "bujo/exec.hpp"
#include "bujo/path.hpp"
#include "bujo/spreads.hpp"
#include <argh.h>
#include <chrono>
#include <cmath>
#include <date/date.h>
#include <deque>
#include <exception>
#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

void output(std::string_view msg) { std::cout << msg << std::endl; }

void log(std::string_view msg) { std::cerr << msg << std::endl; }

bujo::config::journal_config load_config(argh::parser &cli) {
  std::filesystem::path config_path;
  cli({"-c", "--config"}, bujo::config::default_file_path) >> config_path;
  auto expanded_config_path = bujo::path::expand(config_path);
  return bujo::config::load_from_file(expanded_config_path);
}

std::optional<std::string> pop_arg(std::deque<std::string> &args) {
  if (args.empty()) {
    return std::nullopt;
  }
  std::string command = args.front();
  args.pop_front();
  return command;
}

void spread_command(argh::parser &cli, std::deque<std::string> args) {
  auto cfg = load_config(cli);

  auto sub_command = pop_arg(args);
  if (!sub_command) {
    sub_command = "current";
  }

  std::optional<std::filesystem::path> reference_filename = pop_arg(args);

  std::optional<std::string> spread_config_key;
  for (const auto &spread : cfg.spreads) {
    if (cli[{spread.key}]) {
      spread_config_key = spread.key;
    }
  }

  auto spread_config =
      spread_config_key
          ? bujo::config::spread_config_for_key(cfg, *spread_config_key)
          : bujo::config::default_spread_config(cfg);

  if (sub_command == "current" || sub_command == "c") {
    output(bujo::spreads::current_spread(cfg, spread_config).string());
    return;
  }

  if (sub_command == "next" || sub_command == "n") {
    output(bujo::spreads::next_spread(cfg, spread_config, reference_filename)
               .string());
    return;
  }

  if (sub_command == "previous" || sub_command == "prev" ||
      sub_command == "p") {
    output(
        bujo::spreads::previous_spread(cfg, spread_config, reference_filename)
            .string());
    return;
  }

  throw std::invalid_argument("Unknown sub-command for spread: " +
                              *sub_command);
}

void note_command(argh::parser &cli, std::deque<std::string> args) {
  auto cfg = load_config(cli);

  log("Note command executed");
}

void list_command(argh::parser &cli, std::deque<std::string> args) {
  auto cfg = load_config(cli);

  auto all_documents = bujo::documents::list_all(cfg, false);

  for (const auto &doc : all_documents) {
    output(doc.string());
  }
}

void search_command(argh::parser &cli, std::deque<std::string> args) {
  auto cfg = load_config(cli);

  auto all_documents = bujo::documents::list_all(cfg, true);
  std::vector<std::string> document_names;
  for (const auto &doc : all_documents) {
    document_names.push_back(doc.string());
  }

  auto selected = bujo::exec::fzf_select(document_names);
  std::filesystem::path selected_absolute =
      bujo::config::journal_dir(cfg) / selected;
  output(selected_absolute.string());
}

void sync_command(argh::parser &cli, std::deque<std::string> args) {
  auto cfg = load_config(cli);
  if (!cfg.git.commit_message_template) {
    throw std::runtime_error(
        "Git commit message template is not set in config");
  }
  ::date::local_days now{::date::year_month_day{
      ::date::floor<::date::days>(std::chrono::system_clock::now())}};
  auto message = bujo::date::format(now, *cfg.git.commit_message_template);
  bujo::exec::git_commit(cfg, message);
  bujo::exec::git_pull(cfg);
  bujo::exec::git_push(cfg);
}

void configure_command(argh::parser &cli, std::deque<std::string> args) {
  auto cfg = load_config(cli);
  bujo::exec::editor(cfg, bujo::path::expand(bujo::config::default_file_path));
}

int main(int argc, char *argv[]) {
  try {
    argh::parser cli;

    cli.add_params({"-c", "--config"});

    cli.parse(argv);

    std::deque<std::string> args(cli.pos_args().begin(), cli.pos_args().end());
    pop_arg(args); // Remove the program name

    if (args.empty()) {
      log("No command provided");
      return 1;
    }

    auto command = pop_arg(args);
    if (command == "spread") {
      spread_command(cli, std::move(args));
    } else if (command == "note") {
      note_command(cli, std::move(args));
    } else if (command == "list") {
      list_command(cli, std::move(args));
    } else if (command == "search") {
      search_command(cli, std::move(args));
    } else if (command == "sync") {
      sync_command(cli, std::move(args));
    } else if (command == "configure") {
      configure_command(cli, std::move(args));
    } else {
      throw std::invalid_argument("Unknown command: " + *command);
    }
  } catch (const std::exception &e) {
    log(fmt::format("Error: {}", e.what()));
    return 1;
  }
}
