# frozen_string_literal: true
# typed: true

require "test_helper"

module Bujo
  class TestSearch < Minitest::Test
    def setup
      @search_adapter = Mocktail.of_next(Search::Adapters::Fzf)
    end

    def test_searches_with_fzf_adapter
      config = Config.new(search_adapter: "fzf")
      stubs { @search_adapter.search }.with { "test_file_path" }
      assert_equal "test_file_path", Search.new(config).run
    end
  end

  class TestFzfAdapter < Minitest::Test
    def setup
      super
      @files = List.new.run.join("\n")
      @config = Config.load
      @command = Mocktail.of_next(Util::Command)
    end

    def test_executes_fzf
      stubs { @command.execute(@files) }.with { "test_file_path" }
      assert_equal "test_file_path", Search::Adapters::Fzf.new(@config).search
    end

    def test_strips_whitespace
      stubs { @command.execute(@files) }.with { "test_file_path\n" }
      assert_equal "test_file_path", Search::Adapters::Fzf.new(@config).search
    end
  end
end
