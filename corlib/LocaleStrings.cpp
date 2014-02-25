// Perch Lake Computer System

#include "pch.h"
#include "LocaleStrings.h"
#include "System.Array.hpp"
#include "System.Text.UTF8Encoding.h"
#include "System.Text.StringBuilder.h"

System::String MakeLocaleString(int32 idx)
  {
  using namespace System;
  const char* lstring = idx2string(idx);
  uint32 len = (uint32)strlen(lstring);

  Text::UTF8Encoding encoding;
  CharArray buffer(len + 1, L'\0');

  encoding.GetChars((byte*)lstring, (int)strlen(lstring), buffer.ToPtr(), (int)buffer.Length());
  Text::StringBuilder str;
  str.Append(buffer);
  return str.ToString();
  }

System::IntArray MakeLocaleArray(const int32* gs, sizet len)
  {
  using namespace System;
  sizet i = 0;
  sizet length = 0;
  for(;i < len; ++i)
    {
    if(gs[i] == -1)
      break;
    length++;
    }

  IntArray ret(length);
  for(i = 0; i  < length; ++i)
    ret[i] = gs[i];

  return ret;
  }