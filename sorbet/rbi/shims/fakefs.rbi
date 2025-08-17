# typed: true

module FakeFS
  module FileSystem
    sig { params(path: String, target: T.nilable(String)).void }
    def self.clone(path, target = nil)
    end
  end

  sig { void }
  def self.activate!
  end

  sig { void }
  def self.deactivate!
  end
end
