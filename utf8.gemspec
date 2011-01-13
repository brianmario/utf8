# encoding: utf-8

Gem::Specification.new do |s|
  s.name = %q{utf8}
  s.version = "0.1.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Brian Lopez"]
  s.date = %q{2011-01-12}
  s.email = %q{seniorlopez@gmail.com}
  s.extensions = ["ext/utf8/extconf.rb"]
  s.extra_rdoc_files = [
    "README.rdoc"
  ]
  s.files = [".gitignore", "MIT-LICENSE", "README.rdoc", "Rakefile", "benchmark/active_support.rb", "benchmark/test.txt", "ext/utf8/ext.c", "ext/utf8/ext.h", "ext/utf8/extconf.rb", "ext/utf8/string_scanner_utf8.c", "ext/utf8/string_scanner_utf8.h", "ext/utf8/string_utf8.c", "ext/utf8/string_utf8.h", "ext/utf8/utf8.c", "ext/utf8/utf8.h", "lib/utf8.rb", "lib/utf8/string.rb", "lib/utf8/string_scanner.rb", "utf8.gemspec"]
  s.homepage = %q{http://github.com/brianmario/utf8}
  s.require_paths = ["lib", "ext"]
  s.rubygems_version = %q{1.4.2}
  s.summary = %q{A lightweight UTF8-aware String class meant for use with Ruby 1.8}
  s.test_files = ["spec/spec_helper.rb", "spec/string_scanner_spec.rb", "spec/string_spec.rb"]

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<rake-compiler>, [">= 0.7.5"])
      s.add_development_dependency(%q<rspec>, [">= 0"])
    else
      s.add_dependency(%q<rake-compiler>, [">= 0.7.5"])
      s.add_dependency(%q<rspec>, [">= 0"])
    end
  else
    s.add_dependency(%q<rake-compiler>, [">= 0.7.5"])
    s.add_dependency(%q<rspec>, [">= 0"])
  end
end

