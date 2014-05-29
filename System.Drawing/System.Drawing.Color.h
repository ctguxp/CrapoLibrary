#pragma once

namespace System
  {
  namespace Drawing
    {
    class Color
      {
      enum class ColorType
        {
        Empty=0,
        Known=1,
        ARGB=2,
        Named=4,
        System=8
        };
      public:
        Color(int /*red*/, int /*green*/, int /*blue*/, int alpha = 255);
        Color(const Color& /*color*/);
        ~Color();
        Color& operator=(const Color& /*color*/);
        DWORD ToArgb();
      private:
        static void CheckARGBValues(int /*alpha*/,int /*red*/,int /*green*/,int /*blue*/);
        static void CheckRGBValues(int /*red*/,int /*green*/,int /*blue*/);
        static ArgumentException CreateColorArgumentException(int /*value*/, String /*color*/);
      private:
        DWORD      _value;
        ColorType _state;
      };
    }
  }
