#include "pch.h"
#include "System.IO.MemoryStream.h"
#include "System.h"

namespace System
  {
  namespace IO
    {
    MemoryStream::MemoryStream(uintptr capacity)
      :_canWrite(true)
      ,_expandable(true)
      ,_streamClosed(false)
      ,_allowGetBuffer(true) 
      ,_initialIndex(0)
      ,_position(0)
      ,_capacity(capacity)
      ,_length(0)
      ,_internalBuffer()
      {
      if(_capacity > 0)
        _internalBuffer.Length(_capacity);
      }

    MemoryStream::~MemoryStream()
      {
      }

    void MemoryStream::CheckIfClosedThrowDisposed()
      {
      if(_streamClosed)
        // TODO: throw new ObjectDisposedException ("MemoryStream");
          throw Exception(L"Object disposed exception - Memory Stream");
      }

    uintptr MemoryStream::Capacity() 
      { 
      CheckIfClosedThrowDisposed();
      return _capacity - _initialIndex;
      }
    void MemoryStream::Capacity(uintptr value)
      { 
      CheckIfClosedThrowDisposed();

      if(!_expandable)
        // TODO: throw NotSupportedException(L"Cannot expand this MemoryStream");
          throw SystemException(L"Not supported, cannot expand this Memory Stream");

      if(value < _length)
        throw ArgumentOutOfRangeException(L"value", L"New capacity cannot be negative or less than the current capacity" /*+ value + " " + capacity*/);

      if(!_internalBuffer.IsNull() && value == _internalBuffer.Length())
        return;

      _internalBuffer.Length(value);

      _capacity = value;
      }

    uintptr MemoryStream::Length()
      {
      CheckIfClosedThrowDisposed();
      return _length;
      }

    ByteArray MemoryStream::GetBuffer()
      {
      if(!_allowGetBuffer)
        throw UnauthorizedAccessException();

      return _internalBuffer;
      }

    int MemoryStream::ReadByte() 
		{
			CheckIfClosedThrowDisposed();
			if(_position >= _length)
				return -1;

			return _internalBuffer[_position++];
		}

    uintptr MemoryStream::Seek(uintptr, SeekOrigin)
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    uintptr MemoryStream::Position()
      {
      CheckIfClosedThrowDisposed ();
			return _position - _initialIndex;
      }

    void MemoryStream::Position(uintptr)
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    bool MemoryStream::CanRead()
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    bool MemoryStream::CanSeek()
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    bool MemoryStream::CanWrite()
      {
      return (!_streamClosed && _canWrite);
      }

    void MemoryStream::Flush()
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    int MemoryStream::Read(ByteArray& buffer, int offset, int count)
      {
      if(buffer.IsNull())
				throw ArgumentNullException(L"buffer");

			if(offset < 0 || count < 0)
				throw ArgumentOutOfRangeException(L"offset or count less than zero.");

			if(buffer.Length() - offset < count )
				throw ArgumentException (L"offset+count", L"The size of the buffer is less than offset + count.");

			CheckIfClosedThrowDisposed ();

			if(_position >= _length || count == 0)
				return 0;

			if(_position > _length - count)
				count = (int)_length - _position;

      Array<byte>::Copy(buffer, offset, _internalBuffer, 0, count);

			_position += count;
			return count;
      }

    void MemoryStream::SetLength(uintptr)
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    void MemoryStream::Write(ByteArray&, int, int)
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    }
  }
