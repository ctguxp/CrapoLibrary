#pragma once

namespace System
  {
  namespace Drawing
    {
    class Color : public Object
      {
      public:
      enum class ColorType
        {
        Empty=0,
        Known=1,
        ARGB=2,
        Named=4,
        System=8
        };
      public:
        Color(int red = 0, int green = 0, int blue = 0, int alpha = 0);
        Color(const Color& /*color*/);
        ~Color();
        Color& operator=(const Color& /*color*/);
        DWORD ToArgb();
        static Color FromArgb(int red, int green, int blue);
        static Color FromArgb(int alpha, int red, int green, int blue);
        static Color FromArgb(int argb);
        static Color Transparent();
        byte B() { return (byte)_value; }
        byte G() { return (byte)(_value >> 8); }
        byte R() { return (byte)(_value >> 16); }
        DWORD Value() { return _value; }
        void Value(DWORD value) { _value = value; }
        ColorType State() { return _state; }
        void State(ColorType value) { _state = value; }
        int16 KnownColor() { return _knownColor; }
        void KnownColor(int16 value) { _knownColor = value; }
      private:
        static void CheckARGBValues(int /*alpha*/,int /*red*/,int /*green*/,int /*blue*/);
        static void CheckRGBValues(int /*red*/,int /*green*/,int /*blue*/);
        static ArgumentException CreateColorArgumentException(int /*value*/, String /*color*/);
      private:
        DWORD      _value;
        ColorType  _state;
        int16      _knownColor;
      };
    }
  }
