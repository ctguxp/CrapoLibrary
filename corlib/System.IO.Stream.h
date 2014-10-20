#pragma once
#include "AutoPtr.h"
#include "System.Array.h"
#include "System.IO.SeekOrigin.h"

namespace System
  {
  namespace IO
    {
    class CRAPOCOREDLL_API Stream
      {
      public:
        virtual ~Stream();
        virtual int64 Length() = 0;
        virtual int64 Seek(int64, SeekOrigin) = 0;
        virtual int64 Position() = 0;
        virtual void Position(int64) = 0;
        virtual bool CanRead() = 0;
        virtual bool CanSeek() = 0;
        virtual bool CanWrite() = 0;
        virtual void Close();
        virtual void Flush() = 0;
        virtual int Read(ByteArray& buffer, int offset, int count) = 0;
        virtual int ReadByte();
        virtual void SetLength(int64) = 0;
        virtual void Write(ByteArray&, int, int) = 0;
        virtual void WriteByte(byte /*value*/);
      protected:
        Stream();
      };
    typedef AutoPtr<Stream> GCStream;
    }
  }
