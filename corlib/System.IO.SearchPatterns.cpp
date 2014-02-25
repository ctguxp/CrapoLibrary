#include "pch.h"
#include "System.IO.SearchPatterns.h"

namespace System
  {
  namespace IO
    {
    wchar_t SearchPatterns::WildcardChars[2] = { L'*', L'?' };

    SearchPatterns::SearchPatterns()
      {
      }
    SearchPatterns::~SearchPatterns()
      {
      }
    }
  }
