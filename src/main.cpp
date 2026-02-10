#include "bujo/config.hpp"
#include "bujo/documents.hpp"
#include "bujo/path.hpp"
#include "bujo/spreads.hpp"
#include <_stdio.h>
#include <argh.h>
#include <date/date.h>
#include <deque>
#include <iostream>
#include <optional>
#include <stdexcept>

void output(std::string_view msg) { std::cout << msg << std::endl; }

void log(std::string_view msg) { std::cerr << msg << std::endl; }

void spread_command(argh::parser &cli, std::deque<std::string> pos_args) {
  std::filesystem::path config_path;
  cli({"-c", "--config"}, bujo::config::default_file_path) >> config_path;
  auto expanded_config_path = bujo::path::expand(config_path);
  auto cfg = bujo::config::load_from_file(expanded_config_path);

  std::string sub_command;
  if (pos_args.empty()) {
    sub_command = "current";
  } else {
    sub_command = pos_args.front();
    pos_args.pop_front(); // remove the sub-command from the arguments
  }

  std::optional<std::filesystem::path> reference_filename;
  if (!pos_args.empty()) {
    reference_filename = pos_args.front();
    pos_args.pop_front(); // remove the reference filename from the arguments
  }

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

  throw std::invalid_argument("Unknown sub-command for spread: " + sub_command);
}

void note_command(argh::parser &cli, std::deque<std::string> pos_args) {
  std::filesystem::path config_path;
  cli({"-c", "--config"}, bujo::config::default_file_path) >> config_path;
  auto expanded_config_path = bujo::path::expand(config_path);
  auto cfg = bujo::config::load_from_file(expanded_config_path);

  log("Note command executed");
}

void list_command(argh::parser &cli, std::deque<std::string> pos_args) {
  std::filesystem::path config_path;
  cli({"-c", "--config"}, bujo::config::default_file_path) >> config_path;
  auto expanded_config_path = bujo::path::expand(config_path);
  auto cfg = bujo::config::load_from_file(expanded_config_path);

  auto all_documents = bujo::documents::list_all(cfg);

  for (const auto &doc : all_documents) {
    output(doc.string());
  }
}

void search_command(argh::parser &cli, std::deque<std::string> pos_args) {
  std::filesystem::path config_path;
  cli({"-c", "--config"}, bujo::config::default_file_path) >> config_path;
  auto expanded_config_path = bujo::path::expand(config_path);
  auto cfg = bujo::config::load_from_file(expanded_config_path);

  auto all_documents = bujo::documents::list_all(cfg);

  log("Search command executed");
}

int main(int argc, char *argv[]) {
  try {
    argh::parser cli;

    cli.add_params({"-c", "--config"});

    cli.parse(argv);

    std::deque<std::string> pos_args(cli.pos_args().begin(),
                                     cli.pos_args().end());
    pos_args.pop_front(); // remove the program name

    if (pos_args.empty()) {
      log("No command provided");
      return 1;
    }

    const std::string command = pos_args.front();
    pos_args.pop_front(); // remove the command from the arguments

    if (command == "spread") {
      spread_command(cli, std::move(pos_args));
    } else if (command == "note") {
      note_command(cli, std::move(pos_args));
    } else if (command == "list") {
      list_command(cli, std::move(pos_args));
    } else if (command == "search") {
      search_command(cli, std::move(pos_args));
    } else {
      throw std::invalid_argument("Unknown command: " + command);
    }
  } catch (const std::exception &e) {
    log(std::string("Error: ") + e.what());
    return 1;
  }
}
