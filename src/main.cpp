#include "bujo/config.hpp"
#include "bujo/date.hpp"
#include "bujo/path.hpp"
#include "bujo/span.hpp"
#include "bujo/templates.hpp"
#include <argh.h>
#include <date/date.h>
#include <deque>
#include <iostream>

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

  // select first spread config by default
  std::string spread_config_key = cfg.spreads[0].key;
  // check if a specific spread was invoked
  for (const auto &spread : cfg.spreads) {
    if (cli[{spread.key}]) {
      spread_config_key = spread.key;
    }
  }

  bujo::config::spread_config spread_config;
  for (const auto &spread : cfg.spreads) {
    if (spread.key == spread_config_key) {
      spread_config = spread;
      break;
    }
  }

  std::filesystem::path base_spread_path =
      bujo::path::expand(cfg.journal_root_dir);
  if (cfg.spreads_dir) {
    base_spread_path /= *cfg.spreads_dir;
  }
  if (spread_config.directory) {
    base_spread_path /= *spread_config.directory;
  }

  if (reference_filename) {
    reference_filename = bujo::path::expand(*reference_filename);
    std::error_code ec;
    reference_filename =
        std::filesystem::relative(*reference_filename, base_spread_path, ec);
    if (ec) {
      log(std::format("Reference file {} is not in spread directory {}: {}",
                      reference_filename.value().string(),
                      base_spread_path.string(), ec.message()));
      return;
    }
  }

  date::local_days reference_date;
  if (reference_filename) {
    reference_date =
        bujo::date::parse(spread_config.filename_template, *reference_filename);
  } else {
    reference_date = date::local_days{date::year_month_day{
        date::floor<date::days>(std::chrono::system_clock::now())}};
  }
  auto reference_span = bujo::span::Span::from_template(
      spread_config.filename_template, reference_date);

  if (sub_command == "current" || sub_command == "c") {
    // reference_span is the current span
  } else if (sub_command == "next" || sub_command == "n") {
    reference_date = *reference_span.end() + date::days{1};
    reference_span = bujo::span::Span::from_template(
        spread_config.filename_template, reference_date);
  } else if (sub_command == "previous" || sub_command == "prev" ||
             sub_command == "p") {
    reference_date = *reference_span.start() - date::days{1};
    reference_span = bujo::span::Span::from_template(
        spread_config.filename_template, reference_date);
  } else {
    throw std::invalid_argument("Unknown sub-command for spread: " +
                                sub_command);
  }

  std::filesystem::path spread_path = bujo::templates::render_filename_template(
      spread_config.filename_template, reference_span);

  output((base_spread_path / spread_path).string());
}

void note_command(argh::parser &cli, std::deque<std::string> pos_args) {
  std::string config_path;
  cli({"-c", "--config"}, bujo::config::default_file_path) >> config_path;

  auto cfg = bujo::config::load_from_file(bujo::path::expand(config_path));

  log("Note command executed");
}

void list_command(argh::parser &cli, std::deque<std::string> pos_args) {
  std::string config_path;
  cli({"-c", "--config"}, bujo::config::default_file_path) >> config_path;

  log("List command executed");
}

void search_command(argh::parser &cli, std::deque<std::string> pos_args) {
  std::string config_path;
  cli({"-c", "--config"}, bujo::config::default_file_path) >> config_path;

  log("Search command executed");
}

int main(int argc, char *argv[]) {
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
    log("Unknown command: " + command);
    return 1;
  }
}
