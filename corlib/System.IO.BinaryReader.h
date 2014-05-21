#pragma once
#include "System.IO.Stream.h"
#include "System.Text.Encoding.h"

namespace System
  {
  namespace IO
    {
    class CRAPOCOREDLL_API BinaryReader
      {
      public:
        BinaryReader(Stream* /*stream*/);
        BinaryReader(Stream* /*stream*/, Text::Encoding* /*encoding*/);
        virtual ~BinaryReader();
        virtual bool ReadBoolean();
        virtual byte ReadByte();
        virtual int32 ReadInt32();
      protected:
        virtual void FillBuffer(int /*numBytes*/);
      private:
        bool             _leaveOpen;
        ByteArray        _buffer;
        Stream*          _stream;
        Text::GCEncoding _encoding;
      };
    }
  }
