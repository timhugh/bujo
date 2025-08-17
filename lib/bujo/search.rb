# frozen_string_literal: true

require "open3"
require_relative "config"

module Bujo
  class Search
    class ExecError < StandardError; end

    module Adapters
      def self.resolve(config)
        Adapters::Fzf
      end

      module Fzf
        def self.search(base_directory)
          stdout, _stderr, _status = Open3.capture3("find '#{base_directory}' -type f | fzf --preview 'head -10 {}'")
          # TODO: seems like we should validate the output a little here
          stdout.chomp
        end
      end
    end

    def initialize(config = Config.load)
      @config = config
    end

    def run
      Search::Adapters.resolve(@config).search(@config.base_directory)
    end
  end
end
