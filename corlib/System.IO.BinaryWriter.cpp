#include "pch.h"
#include "System.IO.BinaryWriter.h"
#include "System.Text.UTF8Encoding.h"
#include "Crapo.Security.BitConverterLE.h"

using namespace Crapo;

namespace System
  {
  namespace IO
    {
    BinaryWriter::BinaryWriter(Stream* stream)
      :_leaveOpen(false)
      ,_buffer(16)
      ,_stream(stream)
      ,_encoding(new Text::UTF8Encoding())
      {
      }

    BinaryWriter::~BinaryWriter()
      {
      }

    void BinaryWriter::Write(bool value)
      {
      //if(disposed)
      ///throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");
      _buffer[0] = (byte)(value ? 1 : 0);
      _stream->Write(_buffer, 0, 1);
      }
    void BinaryWriter::Write(byte value)
      {
      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");
      _stream->WriteByte(value);
      }
    void BinaryWriter::Write(int32 value)
      {
      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");
      _buffer[0] = (byte) value;
      _buffer[1] = (byte) (value >> 8);
      _buffer[2] = (byte) (value >> 16);
      _buffer[3] = (byte) (value >> 24);
      _stream->Write(_buffer, 0, 4);
      }
    void BinaryWriter::Write(int64 value)
      {
      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");

      for(int i = 0, sh = 0; i < 8; i++, sh += 8)
        _buffer[i] = (byte)(value >> sh);
      _stream->Write(_buffer, 0, 8);
      }
    void BinaryWriter::Write(float value)
      {
      //if (disposed)
      //throw new ObjectDisposedException ("BinaryWriter", "Cannot write to a closed BinaryWriter");
      ByteArray bytes = Security::BitConverterLE::GetBytes(value);
      _stream->Write(bytes, 0, 4);
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
        _stream->Write(stringBuffer, 0, blen);

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
