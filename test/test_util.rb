# frozen_string_literal: true
# typed: strict

require "test_helper"

module Bujo
  class TestCommand < Minitest::Test
    sig { void }
    def test_executes_commands
      assert_equal "hello", Util::Command.new("echo hello").execute.chomp
    end

    sig { void }
    def test_passes_stdin_to_commands
      assert_equal "hello", Util::Command.new("cat").execute("hello").chomp
    end
  end
end
