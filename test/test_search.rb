# frozen_string_literal: true

require "test_helper"
require "bujo/search"

class MockSearchAdapter
  attr_accessor :stubbed_filename, :captured_directory

  def initialize(stubbed_filename)
    @stubbed_filename = stubbed_filename
  end

  def search(directory)
    @captured_directory = directory
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
    def stub_fzf_result(stdout:, stderr: "", success: true, expected_base_directory: "~/test_directory")
      Open3.stub(:capture3, ->(executed_command) do
        assert_equal "find '#{expected_base_directory}' -type f | fzf --preview 'head -10 {}'", executed_command
        ["#{stdout}", "#{stderr}", Struct.new(:success?).new(success)]
      end) do
        yield
      end
    end

    def test_executes_fzf_on_passed_directory
      stub_fzf_result(stdout: "test_file_path", expected_base_directory: "~/test_directory") do
        assert_equal "test_file_path", Search::Adapters::Fzf.search("~/test_directory")
      end
    end

    def test_strips_whitespace
      stub_fzf_result(stdout: "test_file_path\n", expected_base_directory: "~/test_directory") do
        assert_equal "test_file_path", Search::Adapters::Fzf.search("~/test_directory")
      end
    end
  end
end
