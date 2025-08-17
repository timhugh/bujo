# frozen_string_literal: true

require_relative "lib/bujo/version"

Gem::Specification.new do |spec|
  spec.name = "bujo"
  spec.version = Bujo::VERSION
  spec.authors = ["Tim Heuett"]
  spec.email = ["tim.heuett@gmail.com"]

  spec.summary = "Bujo CLI"
  spec.description = "A CLI for integrating your markdown bullet journal into other applications"
  spec.homepage = "https://github.com/timhugh/bujo"
  spec.license = "MIT"
  spec.required_ruby_version = ">= 3.2.0"

  spec.metadata["allowed_push_host"] = "TODO: Set to your gem server 'https://example.com'"
  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = spec.homepage

  gemspec = File.basename(__FILE__)
  spec.files = IO.popen(%w[git ls-files -z], chdir: __dir__, err: IO::NULL) do |ls|
    ls.readlines("\x0", chomp: true).reject do |f|
      (f == gemspec) ||
        f.start_with?(*%w[bin/ Gemfile .gitignore test/ .standard.yml])
    end
  end
  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.add_dependency "dry-cli", "~> 1.3.0"
  spec.add_dependency "tomlrb", "~> 2.0"
  spec.add_dependency "sorbet-runtime", "~> 0.5"
end
