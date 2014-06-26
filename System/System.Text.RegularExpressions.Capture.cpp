#include "pch.h"
#include "System.Text.RegularExpressions.Capture.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      Capture::Capture(String text, int32 index, int32 length)
        :_index(index)
        ,_length(length)
        ,_text(text)
        {
        }
      Capture::~Capture()
        {
        }
      int32 Capture::Index()
        { 
        return _index; 
        }
      int32 Capture::Length()
        { 
        return _length; 
        }
      String Capture::Value()
        { 
        return _text.Substring(_index, _length); 
        }
      String Capture::ToString()
        {
        return Value();
        }
      String Capture::Text()
        { 
        return _text; 
        }
      }
    }
  }
