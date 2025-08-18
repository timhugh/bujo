# frozen_string_literal: true
# typed: strict

require "test_helper"

module Bujo
  class TestSearch < Minitest::Test
    sig { void }
    def test_searches_with_fzf_adapter
      search_adapter = Mocktail.of_next(Search::Adapters::Fzf)
      stubs { search_adapter.search }.with { "test_file_path" }
      config = Config.new(search_adapter: "fzf")
      assert_equal "test_file_path", Search.new(config).run
    end
  end

  class TestFzfAdapter < Minitest::Test
    sig { void }
    def setup
      super
      @files = T.let(List.new.run.join("\n"), T.nilable(String))
      @config = T.let(Config.load, T.nilable(Bujo::Config))
      T.must(@command = T.let(Mocktail.of_next(Util::Command), T.nilable(Bujo::Util::Command)))
    end

    sig { void }
    def test_executes_fzf
      stubs { T.must(@command).execute(@files) }.with { "test_file_path" }
      assert_equal "test_file_path", Search::Adapters::Fzf.new(T.must(@config)).search
    end

    sig { void }
    def test_strips_whitespace
      stubs { T.must(@command).execute(@files) }.with { "test_file_path\n" }
      assert_equal "test_file_path", Search::Adapters::Fzf.new(T.must(@config)).search
    end
  end
end
