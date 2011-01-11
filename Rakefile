require 'rake' unless defined? Rake

gem 'rake-compiler', '~> 0.7.1'
require "rake/extensiontask"

Rake::ExtensionTask.new('utf8') do |ext|
  ext.cross_compile = true
  ext.cross_platform = ['x86-mingw32', 'x86-mswin32-60']

  ext.lib_dir = File.join 'lib', 'utf8'
end
