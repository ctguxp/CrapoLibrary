#include "pch.h"
#include "System.Drawing.Color.h"
#include "System.Drawing.KnownColor.h"
#include "System.Drawing.KnownColors.h"

namespace System
  {
  namespace Drawing           
    {
    Color::Color(int red, int green, int blue, int alpha)
      :_state(Color::ColorType::ARGB)
      ,_value(0)
      ,_knownColor(0)
      {
      CheckARGBValues(red, green, blue, alpha);
      _value = (int32)((uint32) alpha << 24) + (red << 16) + (green << 8) + blue;
      }
    Color::Color(const Color& color)
      :_state(color._state)
      ,_value(color._value)
      ,_knownColor(color._knownColor)
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
      _knownColor = color._knownColor;
      return *this;
      }
    Color Color::FromArgb(int red, int green, int blue)
		{
			return FromArgb(255, red, green, blue);
		}
    Color Color::FromArgb(int alpha, int red, int green, int blue)
		{
			CheckARGBValues(alpha, red, green, blue);
			Color color;
			color._state = ColorType::ARGB;
			color._value = ((uint32) alpha << 24) + (red << 16) + (green << 8) + blue;
			return color;
		}
    Color Color::FromArgb(int argb)
		{
			return FromArgb ((argb >> 24) & 0x0FF, (argb >> 16) & 0x0FF, (argb >> 8) & 0x0FF, argb & 0x0FF);
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
    Color Color::Transparent()
			{ 
      return KnownColors::FromKnownColor(KnownColor::Transparent);
      }
    }
  }
