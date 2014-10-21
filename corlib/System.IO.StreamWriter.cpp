// Perch Lake Computer System

#include "pch.h"
#include "System.IO.StreamWriter.h"
#include "System.Math.h"
#include "System.Text.Encoding.h"
#include "System.IO.FileStream.h"
#include "System.Exception.h"
#include "System.IO.Enums.h"
#include "System.Array.hpp"
#include "System.Buffer.h"

using namespace System;

namespace System
  {
  namespace IO
    {
    StreamWriter::StreamWriter(String path)
      :_internalEncoding(Text::Encoding::UTF8())
      ,_internalStream()
      ,_byte_buf()
      ,_decode_buf()
      ,_byte_pos(0)
      ,_decode_pos(0)
      ,_iflush(false)
      ,_preamble_done(false)
      {
      _internalStream.Set(new FileStream(path, FileMode::Create, FileAccess::Write, FileShare::Read));
      _internalStream->SetLength(0);
      Initialize(DefaultFileBufferSize);
      }
    /*StreamWriter::StreamWriter(Stream* s, Text::Encoding* e, uint32 bufferSize)
    :_internalStream(s)
    ,_internalEncoding(e)
    ,_defaultBufferSize(1024)
    ,_defaultFileBufferSize(4096)
    ,_minimumBufferSize(256)
    ,_byte_buf()
    ,_decode_buf()
    ,_byte_pos(0)
    ,_decode_pos(0)
    ,_iflush()
    ,_preamble_done(false)
    {
    Init(bufferSize);
    }*/
    StreamWriter::~StreamWriter()
      {
      Flush();
      }
    void StreamWriter::Initialize(int bufferSize)
      {
      uint32 bufSize = Math::Max(bufferSize, (int)_defaultBufferSize);
      _decode_buf.Length(bufSize);
      _byte_buf.Length(_internalEncoding->GetMaxByteCount(bufSize));
      if(_internalStream->CanSeek() && _internalStream->Position() > 0)
        _preamble_done = true;
      }

    void StreamWriter::Write(String value)
      {
      CheckState();

      LowLevelWrite(value);

      if(_iflush)
        FlushCore();
      }

    void StreamWriter::Write(int32 value)
      {
      TextWriter::Write(value);
      }

    void StreamWriter::Write(int64 value)
      {
      TextWriter::Write(value);
      }

    void StreamWriter::Write(Object* value)
      {
      TextWriter::Write(value);
      }

    void StreamWriter::Write(CharArray buffer)
      {
      CheckState();

			if(buffer.IsNull())
				LowLevelWrite(buffer, 0, (int32)buffer.Length());
			if(_iflush)
				FlushCore();
      }

    void StreamWriter::Write(CharArray buffer, int32 index, int32 count) 
      {
      if(buffer.IsNull())
        throw ArgumentNullException(L"buffer");
      if(index < 0)
        throw ArgumentOutOfRangeException(L"index", L"< 0");
      if(count < 0)
        throw ArgumentOutOfRangeException(L"count", L"< 0");
      // re-ordered to avoid possible integer overflow
      if(index > (int32)buffer.Length() - count)
        throw ArgumentException(L"index + count > buffer.Length");

      CheckState();

      LowLevelWrite(buffer, index, count);
      if(_iflush)
        FlushCore();
      }

    void StreamWriter::Write(bool value)
      {
      TextWriter::Write(value);
      }

    void StreamWriter::Write(double value)
      {
      TextWriter::Write(value);
      }

    void StreamWriter::Write(float value)
      {
      TextWriter::Write(value);
      }

    void StreamWriter::Write(wchar_t value)
      {
      CheckState ();

      // the size of decode_buf is always > 0 and
      // we check for overflow right away
      if(_decode_pos >= (int32)_decode_buf.Length())
        Decode();
      _decode_buf[_decode_pos++] = value;
      if(_iflush)
        FlushCore ();
      }

    void StreamWriter::CheckState()
      {
      if(_byte_buf.IsNull())
        throw ArgumentNullException(L"Byte buffer is null");
      }

    void StreamWriter::LowLevelWrite(System::String& s)
      {
      int count = s.Length();
      int index = 0;
      while (count > 0)
        {
        int32 todo = (int32)_decode_buf.Length() - _decode_pos;
        if(todo == 0)
          {
          Decode();
          todo = (int32)_decode_buf.Length();
          }
        if (todo > count)
          todo = count;

        for(int i = 0; i < todo; i ++)
          _decode_buf[i + _decode_pos] = s [i + index];

        count -= todo;
        index += todo;
        _decode_pos += todo;
        }
      }

    void StreamWriter::LowLevelWrite(CharArray buffer, int32 index, int32 count)
      {
      while(count > 0) {
        int32 todo = (int32)_decode_buf.Length() - _decode_pos;
        if(todo == 0)
          {
          Decode();
          todo = (int32)_decode_buf.Length();
          }
        if(todo > count)
          todo = count;
        Buffer::BlockCopy(buffer, index * 2, _decode_buf, _decode_pos * 2, todo * 2);
        count -= todo;
        index += todo;
        _decode_pos += todo;
        }
      }

    void StreamWriter::Decode() 
      {
      if(_byte_pos > 0)
        FlushBytes();
      if(_decode_pos > 0)
        {
        int len = _internalEncoding->GetBytes(_decode_buf, 0, _decode_pos, _byte_buf, _byte_pos);
        _byte_pos += len;
        _decode_pos = 0;
        }
      }	

    void StreamWriter::FlushBytes() 
      {
      // write the encoding preamble only at the start of the stream
      if(!_preamble_done && _byte_pos > 0)
        {
        System::ByteArray preamble = _internalEncoding->GetPreamble();
        if (preamble.Length() > 0)
          _internalStream->Write(preamble, 0, (int)preamble.Length());
        _preamble_done = true;
        }
      _internalStream->Write(_byte_buf, 0, _byte_pos);
      _byte_pos = 0;
      }

    void StreamWriter::FlushCore()
      {
      Decode();
      if(_byte_pos > 0)
        {
        FlushBytes();
        _internalStream->Flush();
        }
      }

    void StreamWriter::Flush()
      {
      CheckState();
      FlushCore();
      }
    }
  }
