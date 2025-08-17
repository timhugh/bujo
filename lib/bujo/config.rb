# frozen_string_literal: true

require "tomlrb"

module Bujo
  class Config
    class ParseError < StandardError; end

    def self.load(filepath = "~/.bujorc")
      new(Tomlrb.load_file(File.expand_path(filepath)))
    rescue Errno::ENOENT
      FileUtils.touch(filepath)
      new
    rescue Tomlrb::ParseError => e
      raise ParseError, "Failed to read config file: #{e.message}"
    end

    attr_reader :search_adapter
    attr_reader :exec_command

    def initialize(options = {})
      set_defaults
      set_options(options)
    end

    def base_directory
      File.expand_path(@base_directory)
    end

    private

    def set_defaults
      @base_directory = "~/.bujo"
      @search_adapter = "fzf"
      @exec_command = nil
    end

    def set_options(options)
      options.each { |key, value| set(key, value) }
    end

    def set(key, value)
      raise ArgumentError, "unexpected configuration: #{key}" unless instance_variable_defined?(:"@#{key}")
      instance_variable_set(:"@#{key}", value)
    end
  end
end
