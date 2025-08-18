# frozen_string_literal: true
# typed: strict

require_relative "../search"

module Bujo
  module CLI
    class Search < Dry::CLI::Command
      extend T::Sig

      desc "Interactively find a file using fzf"

      sig { void }
      def call
        puts Bujo::Search.new.run
      end
    end
  end
end
