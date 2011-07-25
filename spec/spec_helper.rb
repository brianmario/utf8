$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'utf8'
require 'utf8/string_scanner'

require 'rspec' unless defined? RSpec

RSpec.configure do |config|
  config.expect_with :stdlib
  config.alias_example_to :test
end
