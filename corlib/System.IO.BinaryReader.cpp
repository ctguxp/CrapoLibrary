#include "pch.h"
#include "System.IO.BinaryReader.h"
#include "System.Text.UTF8Encoding.h"
#include "System.Exception.h"
#include "System.IO.IOException.h"
#include "System.Math.h"

namespace System
  {
  namespace IO
    {
    BinaryReader::BinaryReader(Stream* stream)
      :_leaveOpen(false)
      ,_buffer()
      ,_stream(stream)
      ,_encoding(new Text::UTF8Encoding())
      {
      _buffer.Length(Math::Min(16, _encoding->GetMaxByteCount(1)));
      }
    BinaryReader::BinaryReader(Stream* stream, Text::Encoding* encoding)
      :_leaveOpen(false)
      ,_buffer()
      ,_stream(stream)
      ,_encoding(encoding)
      {
      if(_encoding.Get() != nullptr)
        {
        _encoding.RescindOwnership();
        _buffer.Length(Math::Min(16, _encoding->GetMaxByteCount(1)));
        }
      }
    BinaryReader::~BinaryReader()
      {
      }
    bool BinaryReader::ReadBoolean()
      {
      // Return value:
      //  true if the byte is non-zero; otherwise false.
      return ReadByte() != 0;
      }
    byte BinaryReader::ReadByte()
      {
      /*TODO if (m_stream == null) {
      if (m_disposed)
      throw new ObjectDisposedException ("BinaryReader", "Cannot read from a closed BinaryReader.");

      throw new IOException ("Stream is invalid");
      }*/

      int val = _stream->ReadByte();
      if (val != -1)
        return (byte)val;

      throw SystemException(L"End of Stream");
      // TODO throw new EndOfStreamException ();
      }
    int32 BinaryReader::ReadInt32()
      {
      FillBuffer(4);

      return(_buffer[0] | (_buffer[1] << 8) | (_buffer[2] << 16) | (_buffer[3] << 24));
      }
    void BinaryReader::FillBuffer(int numBytes)
      {
      if(numBytes > _buffer.Length())
        throw ArgumentOutOfRangeException(L"numBytes");
      //if (m_disposed)
      //throw new ObjectDisposedException ("BinaryReader", "Cannot read from a closed BinaryReader.");
      if(_stream==nullptr)
        throw IOException(L"Stream is invalid");

      /* Cope with partial reads */
      int pos=0;

      while(pos<numBytes)
        {
        int n= _stream->Read(_buffer, pos, numBytes-pos);
        if(n==0)
          {
          throw SystemException(L"End of Stream");
          //throw new EndOfStreamException();
          }

        pos+=n;
        }
      }
    }
  }