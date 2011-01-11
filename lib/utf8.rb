require 'utf8/utf8'

class String
  # Wraps our string in an UTF8-aware helper class
  def as_utf8
    String::UTF8.new(self)
  end

  class UTF8
    def as_raw
      ::String.new(self)
    end
  end
end
