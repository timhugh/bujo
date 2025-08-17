# frozen_string_literal: true
# typed: true

$LOAD_PATH.unshift File.expand_path("../lib", __dir__)
require "debug"
require "fakefs/safe"
require "minitest/autorun"
require "bujo"

class Test < Minitest::Test
  def setup
    example_dir = File.expand_path("test/fs")
    home_dir = File.expand_path("~")
    FakeFS::FileSystem.clone(example_dir, home_dir)
    FakeFS.activate!
  end

  def teardown
    FakeFS.deactivate!
  end
end
