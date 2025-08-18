# frozen_string_literal: true
# typed: strict

require_relative "bujo/core_ext"
require_relative "bujo/cli"
require "dry/cli"

module Bujo
  extend T::Sig

  class Error < StandardError; end

  class ConfigurationError < Error; end

  class ExecutionError < Error; end

  sig { void }
  def self.run
    Dry::CLI.new(CLI).call
  end
end
