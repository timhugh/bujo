# frozen_string_literal: true

module Bujo
  module FileHandlers
    def self.resolve
      config = Config.load

      if config.exec_command
        ExecuteHandler.new(config.exec_command)
      else
        PrintHandler.new
      end
    end

    class PrintHandler
      def call(file_path)
        puts file_path
      end
    end

    class ExecuteHandler
      def initialize(command)
        @command = command
      end

      def call(file_path)
        command = @command.gsub("%s", file_path)
        exec(command)
      end
    end
  end
end
