# frozen_string_literal: true

require_relative "../search"

module Bujo
  module CLI
    class Search < Dry::CLI::Command
      desc "Interactively find a file using fzf"

      def call(*)
        puts Bujo::Search.new.run
      end
    end
  end
end
