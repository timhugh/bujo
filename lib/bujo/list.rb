# frozen_string_literal: true
# typed: true

require_relative "config"

module Bujo
  class List
    def initialize(config = Config.load)
      @config = config
    end

    def run
      Dir.glob("#{@config.base_directory}/**/*").select { |path| File.file?(path) }
    end
  end
end
