# frozen_string_literal: true
# typed: strict

module Bujo
  class Spread
    extend T::Sig

    sig { params(config: Config).void }
    def initialize(config = Config.load)
      @config = config
    end

    sig { params(spread_config_key: Symbol).returns(String) }
    def current(spread_config_key: @config.default_spread)
      ""
    end

    sig { params(spread_config_key: Symbol, current_spread: T.nilable(String)).returns(String) }
    def next(spread_config_key: @config.default_spread, current_spread: nil)
      ""
    end

    sig { params(spread_config_key: Symbol, current_spread: T.nilable(String)).returns(String) }
    def previous(spread_config_key: @config.default_spread, current_spread: nil)
      ""
    end

    private

    sig { params(spread_config: Spread, current_spread: String).returns(DateTime) }
    def date_from_current_spread(spread_config, current_spread)
      DateTime.now
    end
  end
end
