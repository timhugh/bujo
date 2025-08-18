# frozen_string_literal: true
# typed: true

require "open3"

module Bujo
  module Util
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
