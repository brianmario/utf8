$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'utf8'
require 'benchmark'

require 'rubygems'
require 'active_support'


raw = File.read(File.expand_path('../test.txt', __FILE__))
utf8 = raw.as_utf8
as_mb = ActiveSupport::Multibyte::Chars.new(raw)

times = 1000

puts "String::UTF8"
Benchmark.bmbm do |x|
  x.report {
    puts "#length"
    times.times {utf8.length}
  }
  x.report {
    puts "#[index]"
    times.times {utf8[1024]}
  }
  x.report {
    puts "#[-index]"
    times.times {utf8[-1024]}
  }
  x.report {
    puts "#[start, len]"
    times.times {utf8[1024, 1024]}
  }
  x.report {
    puts "#[-start, len]"
    times.times {utf8[-1024, 1024]}
  }
end

puts "\n\nActiveSupport::Multibyte::Chars"
Benchmark.bmbm do |x|
  x.report {
    puts "#length"
    times.times {as_mb.length}
  }
  x.report {
    puts "#[index]"
    times.times {as_mb[1024]}
  }
  x.report {
    puts "#[-index]"
    times.times {as_mb[-1024]}
  }
  x.report {
    puts "#[start, len]"
    times.times {as_mb[1024, 1024]}
  }
  x.report {
    puts "#[-start, len]"
    times.times {as_mb[-1024, 1024]}
  }
end