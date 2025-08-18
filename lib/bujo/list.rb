# frozen_string_literal: true
# typed: strict

require_relative "config"

module Bujo
  class List
    extend T::Sig

    sig { params(config: Config).void }
    def initialize(config = Config.load)
      @config = config
    end

    sig { returns(T::Array[String]) }
    def run
      Dir.glob("#{@config.base_directory}/**/*").select { |path| File.file?(path) }
    end
  end
end
