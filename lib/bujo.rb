# frozen_string_literal: true
# typed: true

require_relative "bujo/cli"
require "dry/cli"

module Bujo
  extend T::Sig
  class Error < StandardError; end

  sig { void }
  def self.run
    Dry::CLI.new(CLI).call
  end
end
