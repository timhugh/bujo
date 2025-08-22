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
      # TODO: validate config
      spread_config = @config.spreads[spread_config_key]
      raise ConfigurationError, "Spread config #{spread_config_key} does not exist" unless spread_config
      current_spread_file = DateTime.now.strftime(spread_config.filename_format)
      File.join(@config.base_directory, current_spread_file)
    end
  end
end
