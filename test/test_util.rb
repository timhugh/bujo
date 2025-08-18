# frozen_string_literal: true
# typed: true

require "test_helper"

module Bujo
  class TestCommand < Minitest::Test
    def test_executes_commands
      assert_equal "hello", Util::Command.new("echo hello").execute.chomp
    end

    def test_passes_stdin_to_commands
      assert_equal "hello", Util::Command.new("cat").execute("hello").chomp
    end
  end
end
