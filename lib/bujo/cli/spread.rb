# frozen_string_literal: true
# typed: strict

require_relative "../spread"

module Bujo
  module CLI
    class Spread < Dry::CLI::Command
      extend T::Sig

      desc "Open a spread for a specific date range"

      sig { void }
      def call
        Spread.new
      end
    end
  end
end
