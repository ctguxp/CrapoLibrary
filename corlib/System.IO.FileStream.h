// Perch Lake Computer System

#pragma once
#include "System.IO.Stream.h"
#include "System.String.h"
#include "System.IO.Enums.h"

namespace System
  {
  namespace IO
    {
    class FileStream : public Stream
      {
      private:
        ByteArray  _buf_recycle;
        ByteArray  _buf;			// the buffer
        String     _name;
        intptr     _append_startpos;
        HANDLE     _handle;
        FileAccess _access;
        bool       _owner;
        bool       _async;
        bool       _canseek;
        bool       _anonymous;
        int        _buf_size;
        int        _buf_length;
        int        _buf_offset;
        bool       _buf_dirty;
        long       _buf_start;
      public:
        // Constructor takes path and file mode
        FileStream(System::String& path, FileMode mode, FileAccess = FileAccess::ReadWrite
          ,FileShare share = FileShare::Read, uint32 bufferSize = 4096
          ,FileOptions options = FileOptions::None);
        // Destructor
        ~FileStream();
        virtual uintptr Length() override;
        virtual uintptr Seek(uintptr, SeekOrigin) override;
        virtual uintptr Position() override;
        virtual void Position(uintptr) override;
        virtual bool CanRead() override;
        virtual bool CanSeek() override;
        virtual bool CanWrite() override;
        virtual void Flush() override;
        virtual void Write(ByteArray&, int, int) override;
        virtual void SetLength(uintptr) override;
        virtual int Read(ByteArray& array, int offset, int count) override;
      protected:
        void Init(String& path, FileMode mode, FileAccess access, FileShare share, uint32 bufferSize, bool anonymous, FileOptions options);
      private:
        String GetSecureFileName(String filename);
        void InitBuffer(uint32 size, bool isZeroSize);
        void FlushBuffer(Stream* st);
        void FlushBuffer();
        void FlushBufferIfDirty();
        void WriteInternal(ByteArray& src, int offset, int count);
        int WriteSegment(ByteArray& src, int src_offset, int count);
        int ReadInternal(ByteArray& dest, int offset, int count);
        int ReadSegment(ByteArray& dest, int dest_offset, int count);
        void RefillBuffer();
        int ReadData(HANDLE handle, ByteArray& buf, int offset, int count);
      };
    }
  }
