#include "pch.h"
#include "System.IO.BinaryWriter.h"
#include "System.Text.UTF8Encoding.h"
#include "Crapo.Security.BitConverterLE.h"

using namespace Crapo;

namespace System
  {
  namespace IO
    {

    BinaryWriter::BinaryWriter(Stream* stream, Text::GCEncoding& encoding)
      :_leaveOpen(false)
      ,_disposed(false)
      ,_buffer(16)
      ,_outStream(stream)
      ,_encoding(encoding)
      {
      if(stream == nullptr) 
        throw ArgumentNullException(L"output");
      if(encoding.Get() == nullptr) 
        throw ArgumentNullException(L"encoding");
      if (!stream->CanWrite())
        throw ArgumentException(L"Stream does not support writing or already closed.");
      }

    BinaryWriter::~BinaryWriter()
      {
      }

    void BinaryWriter::Close() 
      {
      Dispose(true);
      }

    void BinaryWriter::Dispose(bool disposing)
      {
      if(disposing && _outStream != nullptr && _leaveOpen)
        _outStream->Close();

      _buffer.Length(0);
      _encoding.Reset();
      _disposed = true;
      }

    Stream& BinaryWriter::BaseStream()
      {
      Flush();
      return *_outStream;
      }

    void BinaryWriter::Flush() 
      {
      _outStream->Flush();
      }

    void BinaryWriter::Write(bool value)
      {
      //if(disposed)
      ///throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");
      _buffer[0] = (byte)(value ? 1 : 0);
      _outStream->Write(_buffer, 0, 1);
      }
    void BinaryWriter::Write(byte value)
      {
      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");
      _outStream->WriteByte(value);
      }
    void BinaryWriter::Write(int32 value)
      {
      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");
      _buffer[0] = (byte) value;
      _buffer[1] = (byte) (value >> 8);
      _buffer[2] = (byte) (value >> 16);
      _buffer[3] = (byte) (value >> 24);
      _outStream->Write(_buffer, 0, 4);
      }
    void BinaryWriter::Write(int64 value)
      {
      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");

      for(int i = 0, sh = 0; i < 8; i++, sh += 8)
        _buffer[i] = (byte)(value >> sh);
      _outStream->Write(_buffer, 0, 8);
      }
    void BinaryWriter::Write(float value)
      {
      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");
      ByteArray bytes = Security::BitConverterLE::GetBytes(value);
      _outStream->Write(bytes, 0, 4);
      }
    void BinaryWriter::Write(String value)
      {

      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");

      int32 len = _encoding->GetByteCount(value);
      Write7BitEncodedInt(len);

      ByteArray stringBuffer(512);
      int maxCharsPerRound = 512 / _encoding->GetMaxByteCount (1);

      int chpos = 0;
      int chrem = value.Length();
      while(chrem > 0) 
        {
        int cch = (chrem > maxCharsPerRound) ? maxCharsPerRound : chrem;
        int blen = _encoding->GetBytes(value, chpos, cch, stringBuffer, 0);
        _outStream->Write(stringBuffer, 0, blen);

        chpos += cch;
        chrem -= cch;
        }
      }
    void BinaryWriter::Write7BitEncodedInt(int32 value)
      {
      do
        {
        int32 high = (value >> 7) & 0x01ffffff;
        byte b = (byte)(value & 0x7f);

        if (high != 0) 
          {
          b = (byte)(b | 0x80);
          }

        Write(b);
        value = high;
        }while(value != 0);
      }
    }
  }
