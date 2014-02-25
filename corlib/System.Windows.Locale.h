// Perch Lake Computer System

#pragma once
#include "System.Object.h"
#include "System.String.h"

class Locale
{
public:
  Locale();
  ~Locale();
  static System::String GetText(System::String& msg);
  static System::String GetText(const wchar_t* msg);
  static System::String GetText(System::String& fmt, System::ObjectArray& args);
};

