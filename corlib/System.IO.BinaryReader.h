#pragma once
#include "System.IO.Stream.h"
#include "System.Text.Encoding.h"
#include "System.String.h"


namespace System
  {
  namespace IO
    {
    class CRAPOCOREDLL_API BinaryReader
      {
      enum
        {
        MaxBufferSize = 128
        };
      public:
        BinaryReader(Stream* /*stream*/);
        BinaryReader(Stream* /*stream*/, Text::Encoding* /*encoding*/);
        virtual ~BinaryReader();
        virtual bool ReadBoolean();
        virtual byte ReadByte();
        virtual float ReadSingle();
        virtual int32 ReadInt32();
        virtual int64 ReadInt64();
        virtual String ReadString();
      protected:
        virtual void FillBuffer(sizet /*numBytes*/);
        int Read7BitEncodedInt();
      private:
        bool             _leaveOpen;
#pragma warning(disable:4251)
        ByteArray        _buffer;
#pragma warning(default:4251)
        Stream*          _stream;
#pragma warning(disable:4251)
        Text::GCEncoding _encoding;
#pragma warning(default:4251)
      };
    }
  }
