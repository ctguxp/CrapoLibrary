#pragma once
#include "System.IO.Stream.h"
#include "System.Text.Encoding.h"
#include "System.Array.h"

namespace System
  {
  namespace IO
    {
    class CRAPOCOREDLL_API BinaryWriter
      {
      public:
        BinaryWriter(Stream* /*stream*/);
        virtual ~BinaryWriter();
        virtual void Write(bool /*value*/);
        virtual void Write(byte /*value*/);
        virtual void Write(int32 /*value*/);
      private:
        bool             _leaveOpen;
        ByteArray        _buffer;
        Stream*          _stream;
        Text::GCEncoding _encoding;
      };
    }
  }
