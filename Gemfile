# frozen_string_literal: true

source "https://rubygems.org"

gemspec

group :development do
  gem "sorbet", "~> 0.5", require: false
  gem "standard", "~> 1.3", require: false
  gem "tapioca", "~> 0.17", require: false
end

group :test do
  gem "fakefs", "~> 3.0", require: "fakefs/safe"
  gem "minitest", "~> 5.16"
end

group :development, :test do
  gem "debug", "~> 1.0"
  gem "irb"
  gem "rake", "~> 13.0"
end
