# frozen_string_literal: true
# typed: true

require "sorbet-runtime"
require "dry/cli"
require_relative "../version"

module Bujo
  module CLI
    class Version < Dry::CLI::Command
      desc "Print version"

      def call(*)
        print "Bujo version #{Bujo::VERSION}\n"
      end
    end
  end
end
