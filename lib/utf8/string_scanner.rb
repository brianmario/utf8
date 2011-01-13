require 'utf8'

class StringScanner
  # Returns an UTF8-aware version of StringScanner wrapping your original string
  #
  # NOTE: this will lose all state associated with the current StringScanner instance
  # (like the current scan position)
  def as_utf8
    StringScanner::UTF8.new(self.string)
  end

  class UTF8
    # Returns a non-UTF8-aware version of StringScanner wrapping your original string
    #
    # NOTE: this will lose all state associated with the current StringScanner::UTF8 instance
    # (like the current scan position)
    def as_raw
      StringScanner.new(self.string)
    end
  end
end