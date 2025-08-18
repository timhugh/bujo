# frozen_string_literal: true
# typed: strict

require "sorbet-runtime"
require "dry/cli"
require_relative "../version"

module Bujo
  module CLI
    class Version < Dry::CLI::Command
      extend T::Sig

      desc "Print version"

      sig { void }
      def call
        print "Bujo version #{Bujo::VERSION}\n"
      end
    end
  end
end
