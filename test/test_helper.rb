# frozen_string_literal: true
# typed: true

$LOAD_PATH.unshift File.expand_path("../lib", __dir__)
require "debug"
require "mocktail/sorbet"
require "fakefs/safe"
require "minitest/autorun"
require "bujo"

# we don't want tests to ever have access to the real file system
example_dir = File.expand_path("test/fs")
home_dir = File.expand_path("~")
FakeFS::FileSystem.clone(example_dir, home_dir)
FakeFS.activate!

class Minitest::Test
  extend T::Sig
  include Mocktail::DSL

  def teardown
    Mocktail.reset
  end
end
