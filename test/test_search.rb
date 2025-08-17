# frozen_string_literal: true

require "test_helper"
require "bujo/search"

class MockSearchAdapter
  attr_accessor :stubbed_filename

  def initialize(stubbed_filename)
    @stubbed_filename = stubbed_filename
  end

  def search
    @stubbed_filename
  end
end

def stub_search_adapter(stubbed_filename, &block)
  Bujo::Search::Adapters.stub(:resolve, MockSearchAdapter.new(stubbed_filename), &block)
end

module Bujo
  class TestSearch < Minitest::Test
    def test_searches_with_config
      config = Config.create(base_directory: "~/test_directory")
      search = Search.new(config)
      stub_search_adapter("test_file_path") do
        assert_equal "test_file_path", search.run
      end
    end
  end

  class TestFzfAdapter < Minitest::Test
    def stub_list_result(result)
    end

    def stub_fzf_result(stdin:, stdout:, expected_base_directory: "~/test_directory")
      Open3.stub(:popen3, ->(command, &block) do
        assert_equal "fzf --preview 'head 10 {}'", command
        block.call(stdin, stdout)
      end) do
        yield
      end
    end

    def test_executes_fzf
      config = Config.create(base_directory: "~/test_directory")
      stub_fzf_result(stdout: "test_file_path", expected_base_directory: "~/test_directory") do
        assert_equal "test_file_path", Search::Adapters::Fzf.new(config).search
      end
    end

    def test_strips_whitespace
      config = Config.create(base_directory: "~/test_directory")
      stub_fzf_result(stdout: "test_file_path\n", expected_base_directory: "~/test_directory") do
        assert_equal "test_file_path", Search::Adapters::Fzf.new(config).search
      end
    end
  end
end
