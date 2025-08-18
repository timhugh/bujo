# frozen_string_literal: true
# typed: true

require "open3"
require_relative "config"
require_relative "util"

module Bujo
  class Search
    extend T::Sig

    module Adapters
      class Adapter
        extend T::Sig

        sig { returns(String) }
        def search
          raise NotImplementedError, "search method must be implemented"
        end
      end

      class Fzf < Adapter
        sig { params(config: Config).void }
        def initialize(config)
          @config = config
        end

        sig { returns(String) }
        def search
          files = List.new(@config).run
          Util::Command.new("fzf --preview 'head 10 {}'").execute(files.to_s).chomp
        end
      end
    end

    sig { params(config: Config).void }
    def initialize(config = Config.load)
      @config = config
    end

    sig { returns(String) }
    def run
      search_adapter.search
    end

    private

    sig { returns(Search::Adapters::Fzf) }
    def search_adapter
      case @config.search_adapter
      when "fzf"
        Adapters::Fzf.new(@config)
      else
        raise ConfigurationError, "unsupported search adapter #{@config.search_adapter}"
      end
    end
  end
end
