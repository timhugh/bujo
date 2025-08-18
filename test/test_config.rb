# frozen_string_literal: true
# typed: strict

require "test_helper"

module Bujo
  class TestConfig < Minitest::Test
    sig { void }
    def test_default_values
      assert_default_config_values Config.new
    end

    sig { void }
    def test_init_with_values
      FileUtils.mkdir(File.expand_path("~/test_directory"))
      new_values = {
        base_directory: "~/test_directory",
        search_adapter: "ripgrep"
      }
      assert_config_values new_values, Config.new(new_values)
    end

    sig { void }
    def test_rejects_unknown_config
      assert_raises(ConfigurationError) { Config.new(unsupported_key: "value") }
    end

    sig { void }
    def test_loads_default_config_file
      assert_default_config_values Config.load
    end

    sig { void }
    def test_loads_config_from_file
      FileUtils.mkdir(File.expand_path("~/.test_bujo"))
      expected_values = {
        base_directory: "~/.test_bujo",
        search_adapter: "ripgrep"
      }
      assert_config_values expected_values, Config.load("~/valid_config.toml")
    end

    sig { void }
    def test_raises_error_for_missing_config_file
      assert_raises(ConfigurationError) { Config.load("~/missing_config.toml") }
    end

    sig { void }
    def test_raises_error_for_invalid_config_file
      assert_raises(ConfigurationError) { Config.load("~/invalid_config.toml") }
    end

    sig { params(expected_values: T::Hash[Symbol, T.anything], actual_config: Config).void }
    def assert_config_values(expected_values, actual_config)
      refute_nil expected_values, "expected_values is not nil"
      refute_nil actual_config, "actual_config is not nil"

      assert_equal File.expand_path(T.cast(expected_values[:base_directory], String)), actual_config.base_directory
      # TODO: check other values
    end

    sig { params(actual_config: Config).void }
    def assert_default_config_values(actual_config)
      assert_config_values(Config::DEFAULTS, actual_config)
    end
  end
end
