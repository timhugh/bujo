# frozen_string_literal: true

require_relative "bujo/cli"
require "dry/cli"

module Bujo
  class Error < StandardError; end

  def self.run
    Dry::CLI.new(CLI).call
  end
end
