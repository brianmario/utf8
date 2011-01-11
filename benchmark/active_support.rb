$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'utf8'
require 'benchmark'

require 'rubygems'
require 'active_support'


raw = File.read(File.expand_path('../test.txt', __FILE__))
utf8 = raw.as_utf8
as_mb = ActiveSupport::Multibyte::Chars.new(raw)

times = 1

Benchmark.bmbm do |x|
  x.report {
    puts "String::UTF8#length"
    utf8.length
  }

  x.report {
    puts "ActiveSupport::Multibyte::Chars#length"
    as_mb.length
  }
end

puts "\n\n"
Benchmark.bmbm do |x|
  x.report {
    puts "String::UTF8#[start, len]"
    utf8[1024, 1024]
  }

  x.report {
    puts "ActiveSupport::Multibyte::Chars#[start, len]"
    as_mb[1024, 1024]
  }
end