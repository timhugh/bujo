# frozen_string_literal: true
# typed: true

require_relative "../list"

module Bujo
  module CLI
    class List < Dry::CLI::Command
      desc "List all of the notes and spreads in your journal"

      def call(*)
        puts Bujo::List.new.run
      end
    end
  end
end
