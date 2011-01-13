class String
  # Wraps our string in an UTF8-aware helper class
  def as_utf8
    String::UTF8.new(self)
  end

  class UTF8
    VERSION = "0.1.0"

    # Gives you access to the raw non-UTF8-aware version of the string
    def as_raw
      ::String.new(self)
    end

    alias :size  :length
    alias :chars :each_char
    alias :slice :[]
  end
end