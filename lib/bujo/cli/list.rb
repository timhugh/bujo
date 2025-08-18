# frozen_string_literal: true
# typed: strict

require_relative "../list"

module Bujo
  module CLI
    class List < Dry::CLI::Command
      extend T::Sig

      desc "List all of the notes and spreads in your journal"

      sig { void }
      def call
        puts Bujo::List.new.run
      end
    end
  end
end
