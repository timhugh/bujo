# frozen_string_literal: true

require "test_helper"
require "bujo/config"

module Bujo
  class TestConfig < Minitest::Test
    def setup
      @default_values = {
        config_file: nil,
        base_directory: "~/.bujo",
        search_adapter: "fzf",
        exec_command: nil
      }.freeze
    end

    def assert_config_values(expected_values, actual_config)
      %i[config_file base_directory].each do |path_param|
        if expected_values[path_param]
          assert_equal File.expand_path(expected_values[path_param]), actual_config.send(path_param), "#{path_param} does not match"
        else
          assert_nil actual_config.send(path_param), "#{path_param} is not nil"
        end
      end
      %i[search_adapter exec_command].each do |param|
        if expected_values[param]
          assert_equal expected_values[param], actual_config.send(param), "#{param} does not match"
        else
          assert_nil actual_config.send(param), "#{param} is not nil"
        end
      end
    end

    def test_default_values
      assert_config_values @default_values, Config.create
    end

    def test_init_with_values
      new_values = {
        config_file: "~/.test_config",
        base_directory: "~/test_directory",
        search_adapter: "ripgrep",
        exec_command: "echo"
      }.freeze
      assert_config_values new_values, Config.create(new_values)
    end

    def test_rejects_unknown_config
      assert_raises(ArgumentError) { Config.create(unsupported_key: "value") }
    end

    def test_loads_default_config_file
      Tomlrb.stub(:load_file, ->(filepath) do
        assert_equal File.expand_path("~/.bujorc"), filepath
        {}
      end) do
        Config.load
      end
    end

    def test_loads_config_from_file
      expected_values = {
        config_file: "./example/valid_config.toml",
        base_directory: "~/.test_bujo",
        search_adapter: "ripgrep",
        exec_command: "vim %s"
      }
      assert_config_values expected_values, Config.load("./example/valid_config.toml")
    end

    def test_creates_config_and_uses_defaults_if_missing
      FileUtils.stub(:touch, ->(filepath) do
        assert_equal File.expand_path("missing_config.toml"), filepath
      end) do
        assert_config_values(
          @default_values.merge(config_file: "missing_config.toml"),
          Config.load("missing_config.toml")
        )
      end
    end

    def test_raises_error_for_invalid_config_file
      assert_raises(Config::ParseError) { Config.load("./example/invalid_config.toml") }
    end
  end
end
