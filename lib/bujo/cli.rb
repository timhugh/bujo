# frozen_string_literal: true

require "dry/cli"
require_relative "cli/version"

module Bujo
  module CLI
    extend Dry::CLI::Registry

    register "version", Version, aliases: ["v", "-v", "--version"]
  end
end
