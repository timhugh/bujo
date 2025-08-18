# frozen_string_literal: true
# typed: true

require "tomlrb"

module Bujo
  class Config
    extend T::Sig

    sig { returns(String) }
    attr_reader :base_directory

    sig { returns(String) }
    attr_reader :search_adapter

    sig { returns(String) }
    attr_reader :default_spread

    sig { returns(T::Hash[String, Spread]) }
    attr_reader :spreads

    DEFAULTS = {
      base_directory: "~/.bujo",
      search_adapter: "fzf",
      default_spread: "weekly",
      spreads: {
        weekly: {
          filename_format: "spreads/%Y/W%V"
        }
      }
    }.freeze

    sig { params(filepath: String).returns(Config) }
    def self.load(filepath = "~/.bujorc")
      options = load_toml_file(filepath)
      new(options)
    end

    sig { params(options: T::Hash[Symbol, T.untyped]).void }
    def initialize(options = {})
      merged_options = Util.deep_merge(DEFAULTS, options)
      @base_directory = File.expand_path(merged_options.delete(:base_directory))
      @search_adapter = merged_options.delete(:search_adapter)
      @default_spread = merged_options.delete(:default_spread)
      @spreads = merged_options.delete(:spreads).each_with_object({}) do |(name, config), acc|
        acc[name] = Spread.new(config)
      end
      unless merged_options.empty?
        raise ConfigurationError, "Configuration has unexpected keys: #{merged_options.keys.join(", ")}"
      end
    end

    sig { params(filepath: String).returns(T::Hash[Symbol, T.untyped]) }
    private_class_method def self.load_toml_file(filepath)
      file = File.read(File.expand_path(filepath))
      Tomlrb.parse(file).deep_symbolize_keys!
    rescue Tomlrb::ParseError, Errno::ENOENT => e
      raise ConfigurationError, "Failed to read config file: #{e.message}"
    end
  end

  class Spread
    extend T::Sig

    sig { returns(String) }
    attr_reader :filename_format

    def initialize(options)
      @filename_format = options[:filename_format]
    end
  end
end
