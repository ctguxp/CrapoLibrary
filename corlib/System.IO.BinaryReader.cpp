#include "pch.h"
#include "System.IO.BinaryReader.h"
#include "System.Text.UTF8Encoding.h"
#include "System.Exception.h"
#include "System.IO.IOException.h"
#include "System.FormatException.h"
#include "System.Math.h"
#include "System.Text.StringBuilder.h"
#include "Crapo.Security.BitConverterLE.h"

using namespace Crapo;

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
      _buffer.Length(Math::Max(16, _encoding->GetMaxByteCount(1)));
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
        _buffer.Length(Math::Max(16, _encoding->GetMaxByteCount(1)));
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
    int64 BinaryReader::ReadInt64()
      {
      FillBuffer(8);

      uint32 ret_low  = (uint32) (_buffer[0] | (_buffer[1] << 8) | (_buffer[2] << 16) | (_buffer[3] << 24));

      uint32 ret_high = (uint32) (_buffer[4] | (_buffer[5] << 8) | (_buffer[6] << 16) | (_buffer[7] << 24));

      return (int64) ((((uint64) ret_high) << 32) | ret_low);
      }
    float BinaryReader::ReadSingle()
      {
      FillBuffer(4);

      return(Security::BitConverterLE::ToSingle(_buffer, 0));
      }
    String BinaryReader::ReadString() 
      {
      /* Inspection of BinaryWriter-written files
      * shows that the length is given in bytes,
      * not chars
      */
      int32 len = Read7BitEncodedInt();

      if(len < 0)
        throw new IOException(L"Invalid binary file (string len < 0)");

      if(len == 0)
        return String::Empty();

      CharArray charBuffer(_encoding->GetMaxByteCount(MaxBufferSize));
      ByteArray charByteBuffer(MaxBufferSize);

      //
      // We read the string here in small chunks. Also, we
      // Attempt to optimize the common case of short strings.
      //
      using namespace Text;
      GCStringBuilder sb;
      do 
        {
        int32 readLen = Math::Min(MaxBufferSize, len);

        readLen = _stream->Read(charByteBuffer, 0, readLen);
        if(readLen == 0)
          throw SystemException(L"End of Stream");
        //TODO throw new EndOfStreamException();

        int cch = _encoding->GetDecoder()->GetChars(charByteBuffer, 0, readLen, charBuffer, 0);

        if(sb.Get() == nullptr && readLen == len) // ok, we got out the easy way, dont bother with the sb
          return String(charBuffer.ToConstPtr(), 0, cch);

        if(sb.Get() == nullptr)
          // Len is a fairly good estimate of the number of chars in a string
            // Most of the time 1 byte == 1 char
              sb.Set(new StringBuilder(len));

        sb->Append(charBuffer, 0, cch);
        len -= readLen;
        } while (len > 0);

        return sb->ToString();
      }
    void BinaryReader::FillBuffer(sizet numBytes)
      {
      if(numBytes > _buffer.Length())
        throw ArgumentOutOfRangeException(L"numBytes");
      //if (m_disposed)
      //throw new ObjectDisposedException ("BinaryReader", "Cannot read from a closed BinaryReader.");
      if(_stream==nullptr)
        throw IOException(L"Stream is invalid");

      /* Cope with partial reads */
      sizet pos=0;

      while(pos < numBytes)
        {
        int n = _stream->Read(_buffer, (int)pos, (int)(numBytes-pos));
        if(n==0)
          {
          throw SystemException(L"End of Stream");
          //throw new EndOfStreamException();
          }

        pos+=n;
        }
      }
    int BinaryReader::Read7BitEncodedInt() 
      {
      int32 ret = 0;
      int32 shift = 0;
      int32 len;
      byte b;

      for(len = 0; len < 5; ++len) 
        {
        b = ReadByte();

        ret = ret | ((b & 0x7f) << shift);
        shift += 7;
        if ((b & 0x80) == 0)
          break;
        }

      if (len < 5)
        return ret;
      else
        throw FormatException(L"Too many bytes in what should have been a 7 bit encoded Int32.");
      }
    }
  }