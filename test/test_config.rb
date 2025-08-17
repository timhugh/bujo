# frozen_string_literal: true
# typed: true

require "test_helper"

module Bujo
  class TestConfig < Test
    def setup
      super
      @default_values = {
        config_file: nil,
        base_directory: "~/.bujo",
        search_adapter: "fzf"
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
      %i[search_adapter].each do |param|
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
        search_adapter: "ripgrep"
      }.freeze
      assert_config_values new_values, Config.create(new_values)
    end

    def test_rejects_unknown_config
      assert_raises(ArgumentError) { Config.create(unsupported_key: "value") }
    end

    def test_loads_default_config_file
      expected_values = {
        config_file: "~/.bujorc",
        base_directory: "~/.test_bujo",
        search_adapter: "ripgrep"
      }
      assert_config_values expected_values, Config.load
    end

    def test_loads_config_from_file
      expected_values = {
        config_file: "~/.bujorc",
        base_directory: "~/.test_bujo",
        search_adapter: "ripgrep"
      }
      assert_config_values expected_values, Config.load("~/.bujorc")
    end

    def test_creates_config_and_uses_defaults_if_missing
      refute File.exist?("missing_config.toml")
      assert_config_values(
        @default_values.merge(config_file: "missing_config.toml"),
        Config.load("missing_config.toml")
      )
      assert File.exist?("missing_config.toml")
    end

    def test_raises_error_for_invalid_config_file
      assert_raises(Config::ParseError) { Config.load("~/invalid_config.toml") }
    end
  end
end
