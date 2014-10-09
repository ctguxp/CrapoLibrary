#pragma once
#include "System.IO.Stream.h"
#include "Global.Memory.h"

using namespace Global;

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
        int32     _dirty_bytes;
        uintptr   _capacity;
        uintptr   _length;
        SharedPtr<ByteArray> _internalBuffer;
      public:
        MemoryStream(uintptr capacity = 0);
        MemoryStream(const SharedPtr<ByteArray>& /*buffer*/);
        MemoryStream(const SharedPtr<ByteArray>& /*buffer*/, bool /*writable*/);
        MemoryStream(const SharedPtr<ByteArray>& /*buffer*/, int32 /*index*/, int32 /*count*/, bool /*writable*/, bool /*publiclyVisible*/);
        ~MemoryStream();
        uintptr Capacity();
        void Capacity(uintptr);
        virtual ByteArray& GetBuffer();
        virtual uintptr Length() override;
        virtual uintptr Seek(uintptr, SeekOrigin) override;
        virtual uintptr Position() override;
        virtual void Position(uintptr value) override;
        virtual bool CanRead() override;
        virtual bool CanSeek() override;
        virtual bool CanWrite() override;
        virtual void Flush() override;
        virtual int Read(ByteArray& /*buffer*/, int /*offset*/, int /*count*/) override;
        virtual int ReadByte() override;
        virtual void SetLength(uintptr) override;
        virtual void WriteByte(byte value) override;
        virtual void Write(ByteArray&, int, int) override;
      private:
        void InternalConstructor(const SharedPtr<ByteArray>& buffer, int32 index, int32 count, bool writable, bool publicallyVisible);
        void CheckIfClosedThrowDisposed();
        int CalculateNewCapacity(int minimum);
        void Expand(int newSize);
      };
    }
  }
