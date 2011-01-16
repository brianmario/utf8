# encoding: utf-8
require File.expand_path('../spec_helper', __FILE__)

describe StringScanner::UTF8 do
  before(:all) do
    @char_array = ["怎", "麼", "也", "沒", "人", "寫", "了", "這", "個", "嗎"]
    @scanner = StringScanner.new(@char_array.join)
    @utf8_scanner = @scanner.as_utf8
  end

  it "should blow up on invalid utf8 chars" do
    # lets cut right into the middle of a sequence so we know it's bad
    str = @char_array.join
    str.force_encoding('binary') if str.respond_to?(:force_encoding)
    str = str[0..1]
    str.force_encoding('utf-8') if str.respond_to?(:force_encoding)
    scanner = StringScanner.new(str).as_utf8

    lambda {
      scanner.getch
    }.should raise_error(ArgumentError)
  end

  it "should extend StringScanner, adding an as_utf8 method that returns a StringScanner::UTF8 instance" do
    @scanner.should respond_to(:as_utf8)
    @scanner.as_utf8.class.should eql(StringScanner::UTF8)
  end

  it "should allow access to a regular (non-utf8-aware) StringScanner based on it's string" do
    raw = @utf8_scanner.as_raw
    raw.class.should eql(StringScanner)
    raw.string.should eql(@utf8_scanner.string)
  end

  it "#getch should be utf8-aware" do
    i=0
    while char = @utf8_scanner.getch
      char.should eql(@char_array[i])
      i+=1
    end
  end

  it "should be able to be reset" do
    i=0
    while char = @utf8_scanner.getch
      char.should eql(@char_array[i])
      if i == 4
        break
      end
      i+=1
    end

    @utf8_scanner.reset

    i=0
    while char = @utf8_scanner.getch
      char.should eql(@char_array[i])
      i+=1
    end
  end
end