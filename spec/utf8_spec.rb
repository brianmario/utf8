# encoding: utf-8
require File.expand_path('../spec_helper', __FILE__)

describe String::UTF8 do
  before(:all) do
    @char_array = ["怎", "麼", "也", "沒", "人", "寫", "了", "這", "個", "嗎"]
    @str = @char_array.join
    @utf8 = @str.as_utf8
    @utf8_len = @char_array.size
  end

  it "should extend String, adding an as_utf8 method that wraps the string" do
    "".should respond_to(:as_utf8)
    "".as_utf8.class.should eql(String::UTF8)
  end

  it "should allow access to the underlying raw string" do
    raw = @utf8.as_raw
    raw.class.should eql(String)
    if defined? Encoding
      raw.length.should eql(@utf8_len)
    else
      raw.length.should eql(@char_array.join.size)
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
      joined.force_encoding('utf-8') if defined? Encoding
      @utf8.should eql(joined)
      @utf8.chars.to_a.size.should eql(@utf8_len)
    end
  end

  context "#[]" do
    it "character by index should be utf8-aware" do
      @char_array.each_with_index do |char, i|
        utf8_char = @utf8[i]
        utf8_char = utf8_char.force_encoding('utf-8') if defined? Encoding
        utf8_char.should eql(char)
      end
    end

    it "substring by start and length should be utf8-aware" do
      @utf8[1, 4].should eql(@char_array[1, 4].join)
      @utf8[0, 6].should eql(@char_array[0, 6].join)

      @utf8[6, 6].should eql(@char_array[6, 6].join)

      # we don't support negative starting indices yet
      lambda {
        @utf8[-1, 2].should eql(@char_array[-1, 2].join)
      }.should raise_error(ArgumentError)
    end
  end
end