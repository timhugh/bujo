# frozen_string_literal: true

require "test_helper"
require "bujo/config"

module Bujo
  class TestConfig < Minitest::Test
    def test_default_values
      config = Config.new
      assert_equal File.expand_path("~/.bujo"), config.base_directory
    end

    def test_init_with_values
      config = Config.new(base_directory: "~/test_directory")
      assert_equal File.expand_path("~/test_directory"), config.base_directory
    end

    def test_rejects_unknown_config
      assert_raises(ArgumentError) { Config.new(unsupported_key: "value") }
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
      loaded_config = Config.load("test/valid_config.toml")
      assert_equal File.expand_path("~/.test_bujo"), loaded_config.base_directory
    end

    def test_uses_defaults_if_config_file_is_missing
      config = Config.load("test/missing_config.toml")
      assert_equal File.expand_path("~/.bujo"), config.base_directory
    end

    def test_raises_error_for_invalid_config_file
      assert_raises(Config::ParseError) { Config.load("test/invalid_config.toml") }
    end
  end
end
