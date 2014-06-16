#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.AnchorInfo.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        AnchorInfo::AnchorInfo(Syntax::Expression& expr, int width)
          :_expr(&expr)
          ,_pos(Position::Any)
          ,_offset(0)
          ,_str(nullptr)
          ,_width(width)
          ,_ignore(false)
          {
          }
        AnchorInfo::AnchorInfo(const AnchorInfo& anchor)
          :_expr(anchor._expr)
          ,_pos(anchor._pos)
          ,_offset(anchor._offset)
          ,_str(anchor._str)
          ,_width(anchor._width)
          ,_ignore(anchor._ignore)
          {
          }
        AnchorInfo::~AnchorInfo()
          {
          }
        AnchorInfo& AnchorInfo::operator=(const AnchorInfo& anchor)
          {
          if(this == &anchor)
            return *this;

          _expr = anchor._expr;
          _pos = anchor._pos;
          _offset = anchor._offset;
          _str = anchor._str;
          _width = anchor._width;
          _ignore = anchor._ignore;

          return *this;
          }
        Expression& AnchorInfo::Expression()
          {
          return *_expr;
          }
        bool AnchorInfo::IsPosition()
          {
          return _pos != Position::Any; 
          }
        int AnchorInfo::Offset()
          {
          return _offset;
          }
        int AnchorInfo::Width()
          { 
          return _width;
          }
        int AnchorInfo::Length()
          { 
          return (_str != nullptr) ? _str->Length() : 0;
          }
        RegularExpressions::Position AnchorInfo::Position()
          {
          return _pos;
          }
        bool AnchorInfo::IgnoreCase()
          {
          return _ignore; 
          }
        bool AnchorInfo::IsSubstring()
          {
          return _str != nullptr;
          }
        String& AnchorInfo::Substring()
          {
          return *_str;
          }
        }
      }
    }
  }
