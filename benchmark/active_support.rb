$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'utf8'
require 'benchmark'

require 'rubygems'
require 'active_support'

$KCODE = 'UTF8'

raw = File.read(File.expand_path('../test.txt', __FILE__))
utf8 = raw.as_utf8
as_mb = ActiveSupport::Multibyte::Chars.new(raw)

times = 1000

puts "String::UTF8"
Benchmark.bmbm { |x|
  x.report("#length") {
    times.times {utf8.length}
  }
  x.report("#[index]") {
    times.times {utf8[1024]}
  }
  x.report("#[-index]") {
    times.times {utf8[-1024]}
  }
  x.report("#[start, len]") {
    times.times {utf8[1024, 1024]}
  }
  x.report("#[-start, len]") {
    times.times {utf8[-1024, 1024]}
  }
  x.report("#clean") {
    times.times {utf8.clean}
  }
  x.report("#valid?") {
    times.times {utf8.valid?}
  }
}

puts "\n\nActiveSupport::Multibyte"
Benchmark.bmbm { |x|
  x.report("#length") {
    times.times {as_mb.length}
  }
  x.report("#[index]") {
    times.times {as_mb[1024]}
  }
  x.report("#[-index]") {
    times.times {as_mb[-1024]}
  }
  x.report("#[start, len]") {
    times.times {as_mb[1024, 1024]}
  }
  x.report("#[-start, len]") {
    times.times {as_mb[-1024, 1024]}
  }
  x.report("ActiveSupport::Multibyte.clean") {
    times.times {ActiveSupport::Multibyte.clean(raw)}
  }
  x.report("ActiveSupport::Multibyte.verify") {
    times.times {ActiveSupport::Multibyte.verify(raw)}
  }
}

require 'iconv'
module ActiveSupport::Multibyte
  class << self
    OUTSIDE_ASCII = /[^\x00-\x7f]/n
    ICONV_CLEANER = Iconv.new('UTF-8//IGNORE', 'UTF-8')

    def clean_with_iconv(string)
      if string =~ OUTSIDE_ASCII
        ICONV_CLEANER.iconv(string + ' ')[0..-2]
      else
        string
      end
    end

    alias_method :clean_without_iconv, :clean
    alias_method :clean, :clean_with_iconv
  end
end
puts "\n\nActiveSupport::Multibyte (patched with Iconv)"
Benchmark.bmbm { |x|
  x.report("ActiveSupport::Multibyte.clean") {
    times.times {ActiveSupport::Multibyte.clean(raw)}
  }
}