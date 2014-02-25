// Perch Lake Computer System

#include "pch.h"
#include "System.Windows.Locale.h"

Locale::Locale()
  {
  }

Locale::~Locale()
  {
  }

System::String Locale::GetText(System::String& msg)
  {
  return msg;
  }

System::String Locale::GetText(const wchar_t* msg)
  {
  return System::String(msg);
  }

System::String Locale::GetText(System::String& fmt, System::ObjectArray& args)
  {
  return System::String::Format(fmt, args);
  }
