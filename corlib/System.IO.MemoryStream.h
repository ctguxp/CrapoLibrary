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
        int32     _capacity;
        int32     _length;
        SharedPtr<ByteArray> _internalBuffer;
      public:
        MemoryStream(int32 capacity = 0);
        MemoryStream(const SharedPtr<ByteArray>& /*buffer*/);
        MemoryStream(const SharedPtr<ByteArray>& /*buffer*/, bool /*writable*/);
        MemoryStream(const SharedPtr<ByteArray>& /*buffer*/, int32 /*index*/, int32 /*count*/);
        MemoryStream(const SharedPtr<ByteArray>& /*buffer*/, int32 /*index*/, int32 /*count*/, bool /*writable*/, bool /*publiclyVisible*/);
        ~MemoryStream();
        int32 Capacity();
        void Capacity(int32);
        virtual ByteArray& GetBuffer();
        virtual int64 Length() override;
        virtual int64 Seek(int64, SeekOrigin) override;
        virtual int64 Position() override;
        virtual void Position(int64 value) override;
        virtual bool CanRead() override;
        virtual bool CanSeek() override;
        virtual bool CanWrite() override;
        virtual void Close() override;
        virtual void Flush() override;
        virtual int Read(ByteArray& /*buffer*/, int32 /*offset*/, int32 /*count*/) override;
        virtual int32 ReadByte() override;
        virtual void SetLength(int64 /*value*/) override;
        virtual ByteArray ToArray();
        virtual void WriteByte(byte value) override;
        virtual void Write(ByteArray& /*buffer*/, int32 /*offset*/, int32 /*count*/) override;
      private:
        void InternalConstructor(const SharedPtr<ByteArray>& buffer, int32 index, int32 count, bool writable, bool publicallyVisible);
        void CheckIfClosedThrowDisposed();
        int CalculateNewCapacity(int minimum);
        void Expand(int newSize);
      };
    }
  }
