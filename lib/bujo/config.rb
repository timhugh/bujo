# frozen_string_literal: true
# typed: true

require "tomlrb"

module Bujo
  # Config is an immutable container for configuration options.
  # It can be created manually from options using Config::create or built from a Toml config file
  # using Config::load.
  Config = Struct.new(
    :config_file,
    :base_directory,
    :search_adapter,
    keyword_init: true
  ) do
    extend T::Sig

    private_class_method :new

    sig { params(filepath: T.nilable(String)).returns(Config) }
    def self.load(filepath = "~/.bujorc")
      absolute_filepath = File.expand_path(filepath)
      options = load_toml_file(absolute_filepath)
      create(config_file: absolute_filepath, **options).freeze
    end

    sig { params(options: T::Hash[String, String]).returns(Config) }
    def self.create(options = {})
      defaults = {
        config_file: nil,
        base_directory: "~/.bujo",
        search_adapter: "fzf"
      }

      new(**defaults.merge(options))
    end

    alias_method :raw_config_file, :config_file
    alias_method :raw_base_directory, :base_directory

    sig { returns(T.nilable(String)) }
    def config_file
      @_expanded_config ||= raw_config_file &&
        raw_config_file != "" &&
        File.expand_path(raw_config_file) ||
        nil
    end

    sig { returns(T.nilable(String)) }
    def base_directory
      @_expanded_base_directory ||= raw_base_directory &&
        raw_base_directory != "" &&
        File.expand_path(raw_base_directory) ||
        nil
    end

    sig { returns(Config) }
    def freeze
      # cache computed values before freezing
      config_file
      base_directory

      super
      self
    end

    sig { params(filepath: String).returns(T::Hash[String, String]) }
    private_class_method def self.load_toml_file(filepath)
      file = File.read(File.expand_path(filepath))
      Tomlrb.parse(file)
    rescue Errno::ENOENT
      FileUtils.touch(File.expand_path(filepath))
      {}
    rescue Tomlrb::ParseError => e
      raise ConfigurationError, "Failed to read config file: #{e.message}"
    end
  end
end
