# frozen_string_literal: true
# typed: strict

require "dry/cli"
require_relative "cli/version"
require_relative "cli/search"
require_relative "cli/list"
require_relative "cli/spread"

module Bujo
  module CLI
    extend Dry::CLI::Registry

    register "version", Version, aliases: ["v", "-v", "--version"]
    register "search", Search, aliases: ["s"]
    register "list", List, aliases: ["l"]
    register "spread", Spread
  end
end
