# frozen_string_literal: true
# typed: true

require "open3"

module Bujo
  module Util
    extend T::Sig

    sig { params(hash1: T::Hash[Symbol, T.untyped], hash2: T::Hash[Symbol, T.untyped]).returns(T::Hash[Symbol, T.untyped]) }
    def self.deep_merge(hash1, hash2)
      hash1.merge(hash2) do |key, old_value, new_value|
        if old_value.is_a?(Hash) && new_value.is_a?(Hash)
          deep_merge(old_value, new_value)
        else
          new_value
        end
      end
    end

    class Command
      extend T::Sig

      sig { params(command: String).void }
      def initialize(command)
        @command = command
      end

      sig { params(stdin: T.nilable(String)).returns(String) }
      def execute(stdin = nil)
        stdin_writer, stdout_reader, stderr_reader, wait_thread = Open3.popen3(@command)

        if stdin
          stdin_writer.puts stdin
        end
        stdin_writer.close
        wait_thread.join

        result = stdout_reader.read
        stdout_reader.close
        stderr_reader.close

        result
      end
    end
  end
end
