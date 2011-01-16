# encoding: utf-8
require File.expand_path('../spec_helper', __FILE__)

describe String::UTF8 do
  before(:all) do
    @char_array = ["怎", "麼", "也", "沒", "人", "寫", "了", "這", "個", "嗎"]
    @str = @char_array.join
    @utf8 = @str.as_utf8
    @utf8_len = @char_array.size
  end

  it "should blow up on invalid utf8 chars" do
    # lets cut right into the middle of a sequence so we know it's bad
    @str.force_encoding('binary') if @str.respond_to?(:force_encoding)
    utf8 = @str[0..1]
    utf8.force_encoding('utf-8') if utf8.respond_to?(:force_encoding)
    utf8 = utf8.as_utf8

    lambda {
      utf8.length
    }.should raise_error(ArgumentError)

    lambda {
      utf8[0, 10]
    }.should raise_error(ArgumentError)

    lambda {
      utf8.chars.to_a
    }.should raise_error(ArgumentError)
  end

  it "should extend String, adding an as_utf8 method that returns a String::UTF8 instance" do
    "".should respond_to(:as_utf8)
    "".as_utf8.class.should eql(String::UTF8)
  end

  it "should allow access to the underlying raw string" do
    raw = @utf8.as_raw
    raw.class.should eql(String)
    if defined? Encoding
      raw.length.should eql(@utf8_len)
    else
      raw.length.should eql(@str.size)
    end
  end

  it "should wrap all returned strings to be utf8-aware" do
    @utf8[0].class.should eql(String::UTF8)
    @utf8.chars.to_a[0].class.should eql(String::UTF8)
  end

  context "#length and #size" do
    it "should be utf8-aware" do
      @utf8.length.should eql(@utf8_len)
      @utf8.size.should eql(@utf8_len)
    end
  end

  context "#chars and #each_char" do
    it "should be utf8-aware" do
      klass = begin
        if defined? Encoding
          Enumerator
        else
          Enumerable::Enumerator
        end
      end

      @utf8.chars.class.should eql(klass)
      i=0
      @utf8.chars do |char|
        char.should_not be_nil
        i+=1
      end
      joined = @utf8.chars.to_a.join
      @utf8.should eql(joined)
      @utf8.chars.to_a.size.should eql(@utf8_len)
      @utf8.chars.to_a.should eql(@char_array)
    end
  end

  context "[offset] syntax" do
    it "should be utf8-aware" do
      @char_array.each_with_index do |char, i|
        utf8_char = @utf8[i]
        utf8_char.should eql(char)
      end
    end

    it "should support negative indices" do
      utf8_char = @utf8[-5]
      utf8_char.should eql(@char_array[-5])
    end

    it "should return nil for out of range indices" do
      @utf8[100].should be_nil
      @utf8[-100].should be_nil
    end
  end

  context "[offset, length] syntax" do
    it "should be utf8-aware" do
      utf8_char = @utf8[1, 4]
      utf8_char.should eql(@char_array[1, 4].join)

      utf8_char = @utf8[0, 6]
      utf8_char.should eql(@char_array[0, 6].join)

      # this will fail due to a bug in 1.9
      unless defined? Encoding
        utf8_char = @utf8[6, 100]
        utf8_char.should eql(@char_array[6, 100].join)
      end

      utf8_char = @utf8[-1, 2]
      utf8_char.should eql(@char_array[-1, 2].join)

      utf8_char = @utf8[-1, 100]
      utf8_char.should eql(@char_array[-1, 100].join)

      utf8_char = @utf8[0, 0]
      utf8_char.should eql(@char_array[0, 0].join)
    end

    it "should return nil for an out of range offset or length" do
      @utf8[100, 100].should be_nil
      @utf8[-100, 100].should be_nil
      @utf8[0, -100].should be_nil
    end
  end

  context "[Range] syntax" do
    it "should be utf8-aware" do
      utf8_char = @utf8[1..4]
      utf8_char.should eql(@char_array[1..4].join)

      utf8_char = @utf8[0..6]
      utf8_char.should eql(@char_array[0..6].join)

      # this will fail due to a bug in 1.9
      unless defined? Encoding
        utf8_char = @utf8[6..100]
        utf8_char.should eql(@char_array[6..100].join)
      end

      utf8_char = @utf8[-1..2]
      utf8_char.should eql(@char_array[-1..2].join)

      utf8_char = @utf8[-1..100]
      utf8_char.should eql(@char_array[-1..100].join)
    end

    it "should return nil for an out of range offset or length" do
      @utf8[100..100].should be_nil
      @utf8[-100..100].should be_nil
      @utf8[0..-100].should eql("")
    end
  end

  it "[Regexp] syntax shouldn't be supported yet" do
    lambda {
      @utf8[/a/]
    }.should raise_error(ArgumentError)
  end

  it "[Regexp, match_index] syntax shouldn't be supported yet" do
    lambda {
      @utf8[/(a)/, 1]
    }.should raise_error(ArgumentError)
  end
end