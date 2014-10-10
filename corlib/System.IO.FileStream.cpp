// Perch Lake Computer System

#include "pch.h"
#include "System.IO.h"
#include "System.Exception.h"
#include "win32.security_attribute.h"
#include "System.Math.h"
#include "System.Buffer.h"
#include "System.Array.hpp"

const int DefaultBufferSize = 4096;

#pragma warning(disable:4100)
namespace System
  {
  namespace IO
    {
    // Constructor takes path and file mode
    FileStream::FileStream(System::String& path, FileMode mode, FileAccess access
      ,FileShare share, uint32 bufferSize, FileOptions options)
      :_buf_recycle()
      ,_buf()
      ,_name(L"[Unknown]")
      ,_handle(NULL)
      ,_access(FileAccess::Read)
      ,_owner(true)
      ,_canseek(false)
      ,_async(false)
      ,_anonymous(false)
      ,_buf_size(0)
      ,_buf_length(0)
      ,_buf_offset(0)
      ,_buf_dirty(false)
      ,_buf_start(0)
      {
      Init(path, mode, access, share, bufferSize, false, options);
      }

    // Destructor
    FileStream::~FileStream()
      {
      Exception* exc = nullptr;
      if(_handle != INVALID_HANDLE_VALUE)
        {
        try
          {
          // If the FileStream is in "exposed" status
          // it means that we do not have a buffer(we write the data without buffering)
          // therefor we don't and can't flush the buffer becouse we don't have one.
          FlushBuffer ();
          }
        catch(Exception& ex)
          {
          exc = &ex;
          }

        if(_owner)
          {
          ulong error;

          CrapoIO::Close(_handle, error);
          if (error != ERROR_SUCCESS) {
            // don't leak the path information for isolated storage
            throw WinException(GetSecureFileName(_name), error);
            //throw MonoIO.GetException (GetSecureFileName (name), error);
            }
          }
        }
      if(exc != nullptr)
				throw *exc;
      }

    void FileStream::Init(String& path, FileMode mode, FileAccess access, FileShare share, uint32 bufferSize, bool anonymous, FileOptions options)
      {
      if(path.Length() == 0) 
        throw ArgumentException(L"Path is empty");

      // ignore the Inheritable flag
      intptr inherit = (intptr)FileShare::Inheritable;
      intptr tshare = (intptr)share;
      tshare &= ~inherit;
      share = (FileShare)tshare;

      if(bufferSize <= 0)
        throw ArgumentOutOfRangeException(L"bufferSize", L"Positive number required.");

      if(mode < FileMode::CreateNew || mode > FileMode::Append)
        {
        if (anonymous)
          throw ArgumentException(L"mode", L"Enum value was out of legal range.");
        else
          throw ArgumentOutOfRangeException(L"mode", L"Enum value was out of legal range.");
        }

      if (access < FileAccess::Read || access > FileAccess::ReadWrite)
        throw ArgumentOutOfRangeException(L"access", L"Enum value was out of legal range.");

      if( (share < FileShare::None) || 
        ((intptr)share > ((intptr)FileShare::ReadWrite | (intptr)FileShare::Delete)) )
        throw ArgumentOutOfRangeException(L"share", L"Enum value was out of legal range.");

      if(path.IndexOfAny(Path::GetInvalidPathChars()) != -1) 
        throw ArgumentException(L"Name has invalid chars");

      if(Directory::Exists(path)) 
        {
        // don't leak the path information for isolated storage
        // TODO: string msg = Locale.GetText ("Access to the path '{0}' is denied.");
        //throw new UnauthorizedAccessException (String.Format (msg, GetSecureFileName (path, false)));
        }

      // Append streams can't be read (see FileMode docs)
      if( (intptr)(mode == FileMode::Append) && ((intptr)access & (intptr)FileAccess::Read) == (intptr)FileAccess::Read)
        throw ArgumentException(L"Append access can be requested only in write-only mode.");

      if( ((intptr)access & (intptr)FileAccess::Write) == 0 && (mode != FileMode::Open && mode != FileMode::OpenOrCreate)) 
        {
        // TODO: string msg = Locale.GetText ("Combining FileMode: {0} with " + "FileAccess: {1} is invalid.");
        //throw new ArgumentException (string.Format (msg, access, mode));
        }

      // TODO: SecurityManager.EnsureElevatedPermissions (); // this is a no-op outside moonlight

      String dname;
      if(Path::DirectorySeparatorChar() != L'/' && path.IndexOf(L'/') >= 0)
        dname = Path::GetDirectoryName(Path::GetFullPath(path));
      else
        dname = Path::GetDirectoryName(path);
      if(dname.Length() > 0)
        {
        String fp = Path::GetFullPath(dname);
        if(!Directory::Exists(fp))
          {
          // don't leak the path information for isolated storage
          String msg(L"Could not find a part of the path \"{0}\".");
          String fname = (anonymous) ? dname : Path::GetFullPath(path);
          throw DirectoryNotFoundException(String::Format(msg, &fname));
          }
        }

      if(access == FileAccess::Read && mode != FileMode::Create && mode != FileMode::OpenOrCreate && mode != FileMode::CreateNew && !File::Exists(path)) 
        {
        // don't leak the path information for isolated storage
        String msg(L"Could not find file \"{0}\".");
        String fname = GetSecureFileName(path);
        throw FileNotFoundException((cstring)String::Format(msg, &fname));
        // TODO : throw FileNotFoundException(String::Format(msg, &fname), fname);
        }

      // IsolatedStorage needs to keep the Name property to the default "[Unknown]"
      if(!_anonymous)
        _name = path;

      // TODO: demand permissions

      ulong error;

      _handle = CrapoIO::Open(path, mode, access, share, options, error);
      if(_handle == INVALID_HANDLE_VALUE) 
        {
        // don't leak the path information for isolated storage
        throw WinException(GetSecureFileName(path), error);
        //throw MonoIO.GetException(GetSecureFileName(path), error);
        }

      _access = access;
      _owner = true;

      // Can we open non-files by name?

      if(CrapoIO::GetFileType(_handle, error) == CrapoFileType::Disk) 
        {
        _canseek = true;
        _async = (options & FileOptions::Asynchronous) != 0;
        } 


      if(access == FileAccess::Read && _canseek && (bufferSize == DefaultBufferSize))
        {
        /* Avoid allocating a large buffer for small files */
        int64 len = Length();
        if (bufferSize > (uint32)len) {
          bufferSize = (uint32)(len < 1000 ? 1000 : len);
          }
        }

      InitBuffer(bufferSize, false);

      if(mode==FileMode::Append) 
        {
        Seek(0, SeekOrigin::End);
        _append_startpos = Position();
        } 
      else 
        {
        _append_startpos=0;
        }
      }

    String FileStream::GetSecureFileName(String filename)
      {
      return _anonymous ? Path::GetFileName(filename) : Path::GetFullPath(filename);
      }

    void FileStream::InitBuffer(uint32 size, bool isZeroSize)
      {
      if(isZeroSize) 
        {
        size = 0;
        _buf.Length(1);
        } 
      else 
        {
        size = Math::Max((int)size, 8);
        }

      //
      // Instead of allocating a new default buffer use the
      // last one if there is any available
      //		
      if(size <= DefaultBufferSize && !_buf_recycle.IsNull())
        {
        //lock (buf_recycle_lock) 
        //{
        if(!_buf_recycle.IsNull()) 
          {
          _buf = _buf_recycle;
          _buf_recycle.Length(0);
          }
        //}
        }

      if(_buf.IsNull())
        _buf.Length(size);
      else
        _buf.Length(0);

      _buf_size = size;
      }

    int64 FileStream::Length()
      {
      if(_handle == INVALID_HANDLE_VALUE)
        throw SystemException(L"Stream has been closed");
      // TODO : throw ObjectDisposedException ("Stream has been closed");

      if(!CanSeek())
        throw SystemException(L"The stream does not support seeking");
      // TODO : throw NotSupportedException ("The stream does not support seeking");

      // Buffered data might change the length of the stream
      FlushBufferIfDirty ();

      ulong error;
      long length;

      length = (long)CrapoIO::GetLength(_handle, error);
      if (error != ERROR_SUCCESS) {
        // don't leak the path information for isolated storage
        throw WinException(GetSecureFileName (_name), error);
        //throw MonoIO.GetException (GetSecureFileName (_name), error);
        }

      return length;
      }

    void FileStream::SetLength(uintptr value)
      {
      }

    int64 FileStream::Seek(int64 offset, SeekOrigin origin)
      {
      if(_handle == INVALID_HANDLE_VALUE)
        throw SystemException(L"Stream has been closed");
      //throw new ObjectDisposedExcept4ion ("Stream has been closed");

      // make absolute

      if(CanSeek() == false)
        {
        throw SystemException(L"The stream does not support seeking");
        //throw new NotSupportedException("The stream does not support seeking");
        }

      int64 pos = 0;
      switch(origin)
        {
        case SeekOrigin::End:
          pos = Length() + offset;
          break;
        case SeekOrigin::Current:
          pos = Position() + offset;
          break;
        case SeekOrigin::Begin:
          pos = offset;
          break;
        default:
          throw new ArgumentException(L"origin", L"Invalid SeekOrigin");
        }

      if(pos < (int64)_append_startpos)
        {
        /* More undocumented crap */
        throw IOException(L"Can't seek back over pre-existing data in append mode");
        }

      FlushBuffer();

      ulong error;
      _buf_start = (ulong)CrapoIO::Seek(_handle, pos, SeekOrigin::Begin, error);

      if (error != ERROR_SUCCESS) 
        {
        // don't leak the path information for isolated storage
        throw WinException(GetSecureFileName (_name), error);
        //throw MonoIO.GetException (GetSecureFileName (name), error);
        }

      return _buf_start;
      }     

    uintptr FileStream::Position()
      {
      if(_handle == INVALID_HANDLE_VALUE)
        throw SystemException(L"Stream has been closed");
      //throw new ObjectDisposedException ("Stream has been closed");

      if(CanSeek() == false)
        {
        throw SystemException(L"The stream does not support seeking");
        //throw new NotSupportedException("The stream does not support seeking");
        }

      //if(safeHandle != null) 
      //{
      // If the handle was leaked outside we always ask the real handle
      //ulong error;

      //long ret = CrapoIO::Seek(_handle, 0,SeekOrigin::Current, error);

      //if (error != ERROR_SUCCESS)
      //  {
      //  // don't leak the path information for isolated storage
      //  throw WinException(GetSecureFileName (_name), error);
      //  //throw MonoIO.GetException (GetSecureFileName (name), error);
      //  }

      //return ret;
      //}

      return(_buf_start + _buf_offset);
      }

    bool FileStream::CanRead()
      {
      return _access == FileAccess::Read || _access == FileAccess::ReadWrite;
      }

    bool FileStream::CanSeek()
      {
      return _canseek;
      }

    bool FileStream::CanWrite()
      {
      return _access == FileAccess::Write || _access == FileAccess::ReadWrite;
      }

    void FileStream::Position(uintptr value)
      {	
      Seek(value, SeekOrigin::Begin);
      }

    void FileStream::Flush()
      {
      if(_handle == INVALID_HANDLE_VALUE)
        throw SystemException(L"Stream has been closed");
      //throw new ObjectDisposedException ("Stream has been closed");

      FlushBuffer ();
      }

    int FileStream::Read(ByteArray& array, int offset, int count)
      {
      if(_handle == INVALID_HANDLE_VALUE)
        throw SystemException(L"Stream has been closed");
      //throw new ObjectDisposedException ("Stream has been closed");
      if(array.IsNull())
        throw ArgumentNullException(L"array");
      if(!CanRead())
        throw SystemException(L"Stream does not support reading");
      //throw new NotSupportedException ("Stream does not support reading");
      int len = (int)array.Length();
      if(offset < 0)
        throw ArgumentOutOfRangeException(L"offset", L"< 0");
      if(count < 0)
        throw ArgumentOutOfRangeException(L"count", L"< 0");
      if(offset > len)
        throw ArgumentException(L"destination offset is beyond array size");
      // reordered to avoid possible integer overflow
      if(offset > len - count)
        throw ArgumentException(L"Reading would overrun buffer");

      /*if (async) {
      IAsyncResult ares = BeginRead (array, offset, count, null, null);
      return EndRead (ares);
      }*/

      return ReadInternal (array, offset, count);
      }

    int FileStream::ReadInternal(ByteArray& dest, int offset, int count)
      {
      int n = ReadSegment(dest, offset, count);
      if (n == count) 
        {
        return count;
        }

      int copied = n;
      count -= n;
      if (count > _buf_size) 
        {
        /* Read as much as we can, up
        * to count bytes
        */
        FlushBuffer();
        n = ReadData(_handle, dest, offset+n, count);

        /* Make the next buffer read
        * start from the right place
        */
        _buf_start += n;
        } 
      else 
        {
        RefillBuffer();
        n = ReadSegment(dest, offset+copied, count);
        }

      return copied + n;
      }

    void FileStream::RefillBuffer()
      {
      FlushBuffer(nullptr);

      _buf_length = ReadData(_handle, _buf, 0, _buf_size);
      }

    int FileStream::ReadData(HANDLE handle, ByteArray& buf, int offset, int count)
      {
      ulong error;
      int amount = 0;

      /* when async == true, if we get here we don't suport AIO or it's disabled
      * and we're using the threadpool */
      amount = CrapoIO::Read(handle, buf, offset, count, error);
      if (error == ERROR_BROKEN_PIPE) 
        {
        amount = 0; // might not be needed, but well...
        } 
      else if (error != ERROR_SUCCESS) 
        {
        // don't leak the path information for isolated storage
        throw WinException(GetSecureFileName (_name), error);
        //throw MonoIO.GetException (GetSecureFileName (name), error);
        }

      /* Check for read error */
      if(amount == -1) 
        {
        throw IOException();
        }

      return(amount);
      }

    int FileStream::ReadSegment(ByteArray& dest, int dest_offset, int count)
      {
      count = Math::Min(count, _buf_length - _buf_offset);

      if (count > 0) {
        // Use the fastest method, all range checks has been done
        Buffer::BlockCopy(_buf, _buf_offset, dest, dest_offset, count);
        _buf_offset += count;
        }

      return count;
      }

    void FileStream::Write(ByteArray& array, int offset, int count)
      {
      if(_handle == INVALID_HANDLE_VALUE)
        throw SystemException(L"Stream has been closed");
      //throw new ObjectDisposedException ("Stream has been closed");
      if(array.IsNull())
        throw ArgumentNullException(L"array");
      if(offset < 0)
        throw ArgumentOutOfRangeException(L"offset", L"< 0");
      if(count < 0)
        throw ArgumentOutOfRangeException(L"count", L"< 0");
      // ordered to avoid possible integer overflow
      if(offset > (int)array.Length() - count)
        throw ArgumentException(L"Reading would overrun buffer");
      if(!CanWrite())
        throw ArgumentException(L"Stream does not support writing");
      //throw NotSupportedException("Stream does not support writing");

      /*if (async) {
      IAsyncResult ares = BeginWrite (array, offset, count, null, null);
      EndWrite (ares);
      return;
      }*/

      WriteInternal(array, offset, count);
      }

    void FileStream::WriteInternal(ByteArray& src, int offset, int count)
      {
      if(count > _buf_size)
        {
        // shortcut for long writes
        ulong error;

        FlushBuffer ();
        int wcount = count;

        while(wcount > 0)
          {
          int n = CrapoIO::Write(_handle, src, offset, wcount, error);
          if (error != ERROR_SUCCESS)
            throw WinException(GetSecureFileName (_name), error);
          //throw MonoIO.GetException (GetSecureFileName (name), error);

          wcount -= n;
          offset += n;
          } 
        _buf_start += count;
        } 
      else 
        {

        int copied = 0;
        while(count > 0) 
          {

          int n = WriteSegment(src, offset + copied, count);
          copied += n;
          count -= n;

          if (count == 0) {
            break;
            }

          FlushBuffer ();
          }
        }
      }

    int FileStream::WriteSegment(ByteArray& src, int src_offset, int count)
      {
      if (count > _buf_size - _buf_offset)
        {
        count = _buf_size - _buf_offset;
        }

      if (count > 0)
        {
        Buffer::BlockCopy(src, src_offset, _buf, _buf_offset, count);
        _buf_offset += count;
        if(_buf_offset > _buf_length)
          {
          _buf_length = _buf_offset;
          }

        _buf_dirty = true;
        }

      return(count);
      }

    void FileStream::FlushBufferIfDirty()
      {
      if(_buf_dirty)
        FlushBuffer(nullptr);
      }

    void FileStream::FlushBuffer()
      {
      FlushBuffer(nullptr);
      }

    void FileStream::FlushBuffer(Stream* st)
      {
      if(_buf_dirty) 
        {
        ulong error;

        if(CanSeek() == true) // TODO : && safeHandle == nullptr)
          {
          CrapoIO::Seek(_handle, _buf_start, SeekOrigin::Begin, error);
          if (error != ERROR_SUCCESS)
            {
            // don't leak the path information for isolated storage
            throw WinException(GetSecureFileName (_name), error);
            //throw MonoIO.GetException (GetSecureFileName (name), error);
            }
          }
        if(st == nullptr) 
          {
          int wcount = _buf_length;
          int offset = 0;
          while (wcount > 0)
            {
            int n = CrapoIO::Write(_handle, _buf, 0, _buf_length, error);
            if (error != ERROR_SUCCESS) {
              // don't leak the path information for isolated storage
              throw WinException(GetSecureFileName (_name), error);
              //throw MonoIO.GetException (GetSecureFileName (name), error);
              }
            wcount -= n;
            offset += n;
            }
          } 
        else 
          {
          st->Write(_buf, 0, _buf_length);
          }
        }

      _buf_start += _buf_offset;
      _buf_offset = _buf_length = 0;
      _buf_dirty = false;
      }


    }
  }
#pragma warning(default:4100)
