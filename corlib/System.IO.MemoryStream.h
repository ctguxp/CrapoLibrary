#pragma once
#include "System.IO.Stream.h"

namespace System
  {
  namespace IO
    {
    class MemoryStream : public Stream
      {
      private:
        bool      _canWrite;
        bool      _expandable;
        bool      _streamClosed;
        bool      _allowGetBuffer;
        int32     _initialIndex;
        int32     _position;
        uintptr   _capacity;
        uintptr   _length;
        ByteArray _internalBuffer;
      public:
        MemoryStream(uintptr capacity = 0);
        ~MemoryStream();
        uintptr Capacity();
        void Capacity(uintptr);
        virtual ByteArray GetBuffer();
        virtual uintptr Length() override;
        virtual uintptr Seek(uintptr, SeekOrigin) override;
        virtual uintptr Position() override;
        virtual void Position(uintptr) override;
        virtual bool CanRead() override;
        virtual bool CanSeek() override;
        virtual bool CanWrite() override;
        virtual void Flush() override;
        virtual int Read(ByteArray& /*buffer*/, int /*offset*/, int /*count*/) override;
        virtual int ReadByte() override;
        virtual void SetLength(uintptr) override;
        virtual void Write(ByteArray&, int, int) override;
      private:
        void CheckIfClosedThrowDisposed();
      };
    }
  }
