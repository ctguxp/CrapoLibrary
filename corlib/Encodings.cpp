// Perch Lake Computer System
#include "pch.h"
#include "Encodings.h"
#include "System.Array.hpp"
#include <cstdio>

System::SByteArray GetCharSet()
  {
  using namespace System;
  SByteArray buf(14, '0');
  sprintf_s(buf.ToPtr(), buf.Length(), "cp%u", ::GetACP ());
  return buf;
  }

System::String* GetCodePage(int& code_page)
  {
  using namespace System;
  code_page = -1;
  SByteArray sb = GetCharSet();
  const char*  charset = sb.ToConstPtr();
  const char* enc = encodings[0];
  int code = 0;
  int want_name = code_page;
  for(int i = 0; enc != 0;)
    {
    if((intptr)enc < 7)
      {
      code = (int)(intptr)enc;
      enc = encodings [++i];
      continue;
      }
    if(strcmp(enc, charset) == 0)
      {
      code_page = code;
      break;
      }
    enc = encodings[++i];
    }

  if(strstr(charset, "utf_8") != nullptr)
    code_page |= 0x10000000;
  if(want_name && code_page == -1)
    {
    CharArray temp(sb.Length());
    ::MultiByteToWideChar(CP_UTF8, 0, sb.ToConstPtr(), (int)sb.Length(), temp.ToPtr(), (int)temp.Length());
    return new String(temp.ToConstPtr());
    }
  else
    return nullptr;
  }