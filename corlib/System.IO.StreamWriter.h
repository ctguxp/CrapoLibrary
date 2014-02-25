// Perch Lake Computer System

#pragma once
#include "System.Array.h"
#include "System.IO.TextWriter.h"
#include "System.IO.Stream.h"

#pragma warning (disable:4512)
namespace System
  {
  class String;
  namespace Text
    {
    class Encoding;
    }
  namespace IO
    {
    class StreamWriter : public TextWriter
      {
      enum
        {
        DefaultBufferSize = 1024,
        DefaultFileBufferSize = 4096,
        MinimumBufferSize = 256
        };
      private:
        Text::Encoding*  _internalEncoding;
        GCStream         _internalStream;

        uint32           _defaultBufferSize;
        uint32           _defaultFileBufferSize;
        uint32           _minimumBufferSize;
        ByteArray        _byte_buf;
        CharArray        _decode_buf;
        int              _byte_pos;
        int              _decode_pos;
        bool             _iflush;
        bool             _preamble_done;
      public:
        StreamWriter(String path);
        //StreamWriter(Stream*, Text::Encoding*, uint32 bufferSize = DefaultBufferSize);
        ~StreamWriter();
        void Write(String&);
        void Flush();
      private:
        void Initialize(int);
        void CheckState();
        void LowLevelWrite(String&);
        void Decode();
        void FlushBytes();
        void FlushCore();
      };
    }
  }
#pragma warning (default:4512)