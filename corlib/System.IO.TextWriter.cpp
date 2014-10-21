#include "pch.h"
#include "System.IO.TextWriter.h"
#include "System.h"

namespace System
  {
  namespace IO
    {
    TextWriter::TextWriter()
      :_coreNewLine(Environment::NewLine().ToCharArray())
      ,_newLine(_coreNewLine.ToConstPtr())
      ,_internalFormatProvider(nullptr)
      {
      }
    TextWriter::TextWriter(IFormatProvider* formatProvider)
      :_coreNewLine(Environment::NewLine().ToCharArray())
      ,_newLine(_coreNewLine.ToConstPtr())
      ,_internalFormatProvider(formatProvider)
      {
      }
    TextWriter::~TextWriter()
      {
      }
    String& TextWriter::NewLine()
      {
      return _newLine = _coreNewLine.ToConstPtr();
      }

    void TextWriter::NewLine(String& value)
      {
      if(value.Length() == 0)
        _coreNewLine = Environment::NewLine().ToCharArray();

      _coreNewLine = value.ToCharArray();
      }

    void TextWriter::Write(bool value)
      {
      Boolean b(value);
      Write(b.ToString());
      }

    void TextWriter::Write(int32 value)
      {
      Int32 i(value);
      Write(i.ToString(_internalFormatProvider));
      }

    void TextWriter::Write(int64 value)
      {
      Int64 i(value);
      Write(i.ToString(_internalFormatProvider));
      }

    void TextWriter::Write(double value)
      {
      Double d(value);
      Write(d.ToString(_internalFormatProvider));
      }

    void TextWriter::Write(float value)
      {
      Single s(value);
      Write(s.ToString(_internalFormatProvider));
      }

    void TextWriter::Write(wchar_t /*value*/)
      {
      // Do nothing
      }

    void TextWriter::Write(String value)
      {
      if(value.Length() != 0)
        Write(value.ToCharArray());
      }

    void TextWriter::Write(Object* value)
      {
      if(value != nullptr)
        Write(value->ToString());
      }

    void TextWriter::Write(CharArray buffer)
      {
      if(buffer.IsNull())
        return;
      Write(buffer, 0, buffer.Length());
      }

    void TextWriter::Write(CharArray buffer, int32 index, int32 count)
      {
      if(buffer.IsNull())
        throw ArgumentNullException(L"buffer");
      if(index < 0 || index > (int32)buffer.Length())
        throw ArgumentOutOfRangeException(L"index");
      // re-ordered to avoid possible integer overflow
      if(count < 0 || (index > (int32)buffer.Length() - count))
        throw ArgumentOutOfRangeException(L"count");

      for(; count > 0; --count, ++index)
        {
        Write(buffer[index]);
        }
      }

    void TextWriter::WriteLine()
      {
      Write(_coreNewLine);
      }

    void TextWriter::WriteLine(bool value)
      {
      Write(value);
      WriteLine();
      }

    void TextWriter::WriteLine(wchar_t value)
      {
      Write(value);
      WriteLine ();
      }

    void TextWriter::WriteLine(CharArray buffer)
      {
      Write(buffer);
      WriteLine ();
      }

    }
  }
