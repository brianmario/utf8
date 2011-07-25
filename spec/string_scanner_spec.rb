# encoding: utf-8
require File.expand_path('../spec_helper', __FILE__)

describe StringScanner::UTF8 do
  before :each do
    @char_array = ["怎", "麼", "也", "沒", "人", "寫", "了", "這", "個", "嗎"]
    @scanner = StringScanner.new(@char_array.join)
    @utf8_scanner = @scanner.as_utf8
  end

  test "should blow up on invalid utf8 chars" do
    # lets cut right into the middle of a sequence so we know it's bad
    str = @char_array.join
    str.force_encoding('binary') if str.respond_to?(:force_encoding)
    str = str[0..1]
    str.force_encoding('utf-8') if str.respond_to?(:force_encoding)
    scanner = StringScanner.new(str).as_utf8

    assert_raise ArgumentError do
      scanner.getch
    end
  end

  test "should extend StringScanner, adding an as_utf8 method that returns a StringScanner::UTF8 instance" do
    assert @scanner.respond_to?(:as_utf8)
    assert_equal StringScanner::UTF8, @scanner.as_utf8.class
  end

  test "should allow access to a regular (non-utf8-aware) StringScanner based on it's string" do
    raw = @utf8_scanner.as_raw
    assert_equal StringScanner, raw.class
    assert_equal @utf8_scanner.string, raw.string
  end

  test "#getch should be utf8-aware" do
    i=0
    while char = @utf8_scanner.getch
      assert_equal @char_array[i], char
      i+=1
    end
  end

  test "should be able to be reset" do
    i=0
    while char = @utf8_scanner.getch
      assert_equal @char_array[i], char
      if i == 4
        break
      end
      i+=1
    end

    @utf8_scanner.reset

    i=0
    while char = @utf8_scanner.getch
      assert_equal @char_array[i], char
      i+=1
    end
  end
end