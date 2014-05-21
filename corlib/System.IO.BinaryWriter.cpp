#include "pch.h"
#include "System.IO.BinaryWriter.h"
#include "System.Text.UTF8Encoding.h"

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
    }
  }
