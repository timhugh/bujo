# frozen_string_literal: true
# typed: strict

require "tomlrb"

module Bujo
  class Config
    extend T::Sig

    sig { returns(String) }
    attr_reader :base_directory

    sig { returns(String) }
    attr_reader :search_adapter

    sig { returns(Symbol) }
    attr_reader :default_spread

    sig { returns(T::Hash[String, Spread]) }
    attr_reader :spreads

    DEFAULTS = T.let({
      base_directory: "~/.bujo",
      search_adapter: "fzf",
      default_spread: "weekly",
      spreads: {
        weekly: {
          filename_format: "spreads/%Y/W%V"
        },
        monthly: {
          filename_format: "spreads/%Y/M%m-%B"
        }
      }
    }.freeze, T::Hash[Symbol, T.untyped])

    sig { params(filepath: String).returns(Config) }
    def self.load(filepath = "~/.bujorc")
      options = load_toml_file(filepath)
      new(options)
    end

    sig { params(options: T::Hash[Symbol, T.untyped]).void }
    def initialize(options = {})
      merged_options = Util.deep_merge(DEFAULTS, options)
      @base_directory = T.let(File.expand_path(merged_options.delete(:base_directory)), String)
      @search_adapter = T.let(merged_options.delete(:search_adapter), String)
      @default_spread = T.let(merged_options.delete(:default_spread).to_sym, Symbol)
      @spreads = T.let(merged_options.delete(:spreads).each_with_object({}) do |(name, config), acc|
        acc[name] = Spread.new(config)
      end, T::Hash[String, Spread])
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

    sig { params(options: T::Hash[Symbol, T.untyped]).void }
    def initialize(options)
      @filename_format = T.let(options[:filename_format], String)
    end
  end
end
