#include "pch.h"
#include "System.IO.TextReader.h"
#include "System.text.StringBuilder.h"

namespace System
  {
  namespace IO
    {
    TextReader::TextReader()
      {
      }
    TextReader::~TextReader()
      {
      }
    int TextReader::Read()
		  {
			return -1;
		  }
    String TextReader::ReadToEnd()
      { 
      using namespace Text;
      StringBuilder result;
      int c;
      while((c = Read()) != -1)
        result.Append((wchar_t) c);
      return result.ToString();
      }
    }
  }
