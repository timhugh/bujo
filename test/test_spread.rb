# frozen_string_literal: true
# typed: strict

require "test_helper"

module Bujo
  class TestSpread < Minitest::Test
  end

  class TestSpreadCurrent < Minitest::Test
    sig { params(args: T.anything).void }
    def initialize(*args)
      super
      @now = T.let(DateTime.new(2025, 8, 21), DateTime)
    end

    sig { void }
    def setup
      Mocktail.replace(DateTime)
      stubs { DateTime.now }.with { @now }
    end

    sig { void }
    def test_returns_current_spread_for_default_config
      assert_equal File.expand_path("~/.bujo/spreads/2025/W34.md"), Spread.new.current
    end

    sig { void }
    def test_returns_current_spread_for_passed_config
      assert_equal File.expand_path("~/.bujo/spreads/2025/M08-August.md"), Spread.new.current(spread_config_key: :monthly)
    end
  end

  class TestSpreadNextAndPrevious < Minitest::Test
  end
end
