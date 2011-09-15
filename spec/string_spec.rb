# encoding: utf-8
require File.expand_path('../spec_helper', __FILE__)

describe String::UTF8 do
  before :each do
    @char_array = ["怎", "麼", "也", "沒", "人", "寫", "了", "這", "個", "嗎"]
    @str = @char_array.join
    @utf8 = @str.as_utf8
    @utf8_len = @char_array.size
    @codepoints = @char_array.map{|c| c.unpack('U').first}
  end

  test "should blow up on invalid utf8 chars" do
    # lets cut right into the middle of a sequence so we know it's bad
    @str.force_encoding('binary') if @str.respond_to?(:force_encoding)
    utf8 = @str[0..1]
    utf8.force_encoding('utf-8') if utf8.respond_to?(:force_encoding)
    utf8 = utf8.as_utf8

    assert_raise ArgumentError do
      utf8.length
    end

    assert_raise ArgumentError do
      utf8[0, 10]
    end

    assert_raise ArgumentError do
      utf8.chars.to_a
    end
  end

  test "should extend String, adding an as_utf8 method that returns a String::UTF8 instance" do
    assert "".respond_to?(:as_utf8)
    assert_equal String::UTF8, "".as_utf8.class
  end

  test "should allow access to the underlying raw string" do
    raw = @utf8.as_raw
    assert_equal String, raw.class
    if defined? Encoding
      assert_equal @utf8_len, raw.length
    else
      assert_equal @str.size, raw.length
    end
  end

  test "should wrap all returned strings to be utf8-aware" do
    assert_equal String::UTF8, @utf8[0].class
    assert_equal String::UTF8, @utf8.chars.to_a[0].class
  end

  test "clean should replace invalid utf8 chars with '?'" do
    orig = "provided by Cristian Rodr\355guez."
    clean = "provided by Cristian Rodr?guez."
    assert_equal clean, orig.as_utf8.clean
    assert_equal "asdf24??asdf24", "asdf24\206\222asdf24".as_utf8.clean
    assert_equal "asdf24?asdf24", "asdf24\342asdf24".as_utf8.clean
    assert_equal "asdf24??asdf24", "asdf24\342\206asdf24".as_utf8.clean
    assert_equal "asdf24?asdf24", "asdf24\222asdf24".as_utf8.clean
  end

  test "clean should not replace valid utf8 chars with '?'" do
    assert_equal "asdf24\342\206\222asdf24", "asdf24\342\206\222asdf24".as_utf8.clean
  end

  context "#length and #size" do
    test "should be utf8-aware" do
      assert_equal @utf8_len, @utf8.length
      assert_equal @utf8_len, @utf8.size
    end
  end

  context "#chars and #each_char" do
    test "should be utf8-aware" do
      klass = begin
        if defined? Encoding
          Enumerator
        else
          Enumerable::Enumerator
        end
      end

      assert_equal klass, @utf8.chars.class
      @utf8.chars do |char|
        assert !char.nil?
      end
      joined = @utf8.chars.to_a.join
      assert_equal joined, @utf8
      assert_equal @utf8_len, @utf8.chars.to_a.size
      assert_equal @char_array, @utf8.chars.to_a
    end
  end

  context "#codepoints and #each_codepoint" do
    test "should be utf8-aware" do
      klass = begin
        if defined? Encoding
          Enumerator
        else
          Enumerable::Enumerator
        end
      end

      assert_equal klass, @utf8.codepoints.class
      @utf8.codepoints do |codepoint|
        assert !codepoint.nil?
      end
      assert_equal @codepoints.size, @utf8.codepoints.to_a.size
      assert_equal @codepoints, @utf8.codepoints.to_a
    end
  end

  context "[offset] syntax" do
    test "should be utf8-aware" do
      @char_array.each_with_index do |char, i|
        utf8_char = @utf8[i]
        assert_equal char, utf8_char
      end
    end

    test "should support negative indices" do
      utf8_char = @utf8[-5]
      assert_equal @char_array[-5], utf8_char
    end

    test "should return nil for out of range indices" do
      assert @utf8[100].nil?
      assert @utf8[-100].nil?
    end
  end

  context "[offset, length] syntax" do
    test "should be utf8-aware" do
      assert_equal @char_array[1, 4].join, @utf8[1, 4]
      assert_equal @char_array[0, 6].join, @utf8[0, 6]

      # this will fail due to a bug in 1.9
      unless defined? Encoding
        assert_equal @char_array[6, 100].join, @utf8[6, 100]
      end

      assert_equal @char_array[-1, 2].join, @utf8[-1, 2]
      assert_equal @char_array[-1, 100].join, @utf8[-1, 100]
      assert_equal @char_array[0, 0].join, @utf8[0, 0]
    end

    test "should return nil for an out of range offset or length" do
      assert @utf8[100, 100].nil?
      assert @utf8[-100, 100].nil?
      assert @utf8[0, -100].nil?
    end
  end

  context "[Range] syntax" do
    test "should be utf8-aware" do
      assert_equal @char_array[1..4].join, @utf8[1..4]
      assert_equal @char_array[0..6].join, @utf8[0..6]

      # this will fail due to a bug in 1.9
      unless defined? Encoding
        assert_equal @char_array[6..100].join, @utf8[6..100]
      end

      assert_equal @char_array[-1..2].join, @utf8[-1..2]
      assert_equal @char_array[-1..100].join, @utf8[-1..100]
    end

    test "should return nil for an out of range offset or length" do
      assert @utf8[100..100].nil?
      assert @utf8[-100..100].nil?
      assert_equal "", @utf8[0..-100]
    end
  end

  context "#valid?" do
    test "should test validity" do
      # lets cut right into the middle of a sequence so we know it's bad
      @str.force_encoding('binary') if @str.respond_to?(:force_encoding)
      utf8 = @str[0..1]
      utf8.force_encoding('utf-8') if utf8.respond_to?(:force_encoding)
      utf8 = utf8.as_utf8

      assert !utf8.valid?
      assert @utf8.valid?

      assert !"provided by Cristian Rodr\355guez.".as_utf8.valid?
    end

    test "should test validity using a maximum codepoint" do
      highest_codepoint = @utf8.codepoints.to_a.max

      assert @utf8.valid?(highest_codepoint)
      assert !@utf8.valid?(highest_codepoint-1)
    end
  end

  test "[Regexp] syntax shouldn't be supported yet" do
    assert_raise ArgumentError do
      @utf8[/a/]
    end
  end

  test "[Regexp, match_index] syntax shouldn't be supported yet" do
    assert_raise ArgumentError do
      @utf8[/(a)/, 1]
    end
  end

  context "#ascii_only" do
    test "should return true for a string within the low ascii range" do
      assert "asdf".as_utf8.ascii_only?
    end

    test "should return false for a string within the low ascii range" do
      assert !@char_array.first.as_utf8.ascii_only?
    end
  end
end