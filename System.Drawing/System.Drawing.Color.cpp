#include "pch.h"
#include "System.Drawing.Color.h"

namespace System
  {
  namespace Drawing           
    {
    Color::Color(int red, int green, int blue, int alpha)
      :_state(Color::ColorType::ARGB)
      ,_value(0)
      {
      CheckARGBValues(red, green, blue, alpha);
      _value = (int32)((uint32) alpha << 24) + (red << 16) + (green << 8) + blue;
      }
    Color::Color(const Color& color)
      :_state(color._state)
      ,_value(color._value)
      {
      }
    Color::~Color()
      {
      }
    Color& Color::operator=(const Color& color)
      {
      if(this == &color)
        return *this;

      _state = color._state;
      _value = color._value;
      return *this;
      }
    DWORD Color::ToArgb()
      {
      return _value;
      } 
    void Color::CheckARGBValues(int alpha,int red,int green,int blue)
      {
      if( (alpha > 255) || (alpha < 0))
        throw CreateColorArgumentException (alpha, "alpha");
      CheckRGBValues(red,green,blue);
      }
    void Color::CheckRGBValues (int red,int green,int blue)
      {
      if( (red > 255) || (red < 0))
        throw CreateColorArgumentException(red, "red");
      if( (green > 255) || (green < 0))
        throw CreateColorArgumentException (green, "green");
      if( (blue > 255) || (blue < 0))
        throw CreateColorArgumentException (blue, "blue");
      }
    ArgumentException Color::CreateColorArgumentException (int value, String color)
      {
      Int32 v(value);
      return ArgumentException(String::Format(L"'{0}' is not a valid value for '{1}'. '{1}' should be greater or equal to 0 and less than or equal to 255.", &v, &color));
      }
    }
  }
