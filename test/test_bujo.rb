# frozen_string_literal: true
# typed: strict

require "test_helper"

class TestBujo < Minitest::Test
  sig { void }
  def test_that_it_has_a_version_number
    refute_nil ::Bujo::VERSION
  end
end
