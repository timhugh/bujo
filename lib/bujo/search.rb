# frozen_string_literal: true

require "open3"
require_relative "config"

module Bujo
  class Search
    class ExecError < StandardError; end

    module Adapters
      def self.resolve(config)
        Adapters::Fzf.new(config)
      end

      class Fzf
        def initialize(config)
          @config = config
        end

        def search
          files = List.new(@config).run
          Open3.popen3("fzf --preview 'head 10 {}'") do |stdin, stdout|
            stdin.puts files
            stdin.close
            stdout.read.chomp
          end
        end
      end
    end

    def initialize(config = Config.load)
      @config = config
    end

    def run
      Search::Adapters.resolve(@config).search
    end
  end
end
