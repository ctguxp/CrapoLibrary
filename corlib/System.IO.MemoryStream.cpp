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
      ,_dirty_bytes(0)
      ,_capacity(capacity)
      ,_length(0)
      ,_internalBuffer(new ByteArray(capacity))
      {
      }

    MemoryStream::MemoryStream(const SharedPtr<ByteArray>& buffer)
      :_streamClosed(false)
      ,_dirty_bytes(0)
      {
      if(buffer->IsNull())
        throw ArgumentNullException(L"buffer");

      InternalConstructor(buffer, 0, (int32)buffer->Length(), true, false);
      }

    MemoryStream::MemoryStream(const SharedPtr<ByteArray>& buffer, bool writable)
      :_streamClosed(false)
      ,_dirty_bytes(0)
      {
      if(buffer->IsNull())
        throw ArgumentNullException(L"buffer");

      InternalConstructor(buffer, 0, (int32)buffer->Length(), writable, false);
      }

    MemoryStream::MemoryStream(const SharedPtr<ByteArray>& buffer, int32 index, int32 count, bool writable, bool publiclyVisible)
      :_streamClosed(false)
      ,_dirty_bytes(0)
      {
      if(buffer->IsNull())
        throw ArgumentNullException(L"buffer");

      InternalConstructor(buffer, index, count, writable, publiclyVisible);
      }

    MemoryStream::~MemoryStream()
      {
      }

    void MemoryStream::InternalConstructor(const SharedPtr<ByteArray>& buffer, int32 index, int32 count, bool writable, bool publicallyVisible)
      {
      if(index < 0 || count < 0)
        throw ArgumentOutOfRangeException(L"index or count is less than 0.");

      if((int32)buffer->Length() - index < count)
        throw ArgumentException (L"index+count",  L"The size of the buffer is less than index + count.");

      _canWrite = writable;

      _internalBuffer = buffer;
      _capacity = count + index;
      _length = _capacity;
      _position = index;
      _initialIndex = index;

      _allowGetBuffer = publicallyVisible;
      _expandable = false;
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

      if(!_internalBuffer->IsNull() && value == _internalBuffer->Length())
        return;

      _internalBuffer->Length(value);

      _capacity = value;
      }

    uintptr MemoryStream::Length()
      {
      CheckIfClosedThrowDisposed();
      return _length;
      }

    ByteArray& MemoryStream::GetBuffer()
      {
      if(!_allowGetBuffer)
        throw UnauthorizedAccessException();

      return (*_internalBuffer.Get());
      }

    int MemoryStream::ReadByte() 
      {
      CheckIfClosedThrowDisposed();
      if(_position >= (int32)_length)
        return -1;

			return (*_internalBuffer.Get())[_position++];
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

    void MemoryStream::Position(uintptr value)
      {
      CheckIfClosedThrowDisposed ();

      if(value > Int32::MaxValue)
        throw new ArgumentOutOfRangeException(L"value", L"Position must be non-negative and less than 2^31 - 1 - origin");

      _position = _initialIndex + (int32)value;
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

			if((int32)buffer.Length() - offset < count )
				throw ArgumentException (L"offset+count", L"The size of the buffer is less than offset + count.");

			CheckIfClosedThrowDisposed ();

			if(_position >= (int32)_length || count == 0)
				return 0;

			if(_position > (int32)_length - count)
				count = (int)_length - _position;

      Array<byte>::Copy(buffer, offset, (*_internalBuffer.Get()), 0, count);

			_position += count;
			return count;
      }

    void MemoryStream::SetLength(uintptr)
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    int MemoryStream::CalculateNewCapacity(int minimum)
      {
      if (minimum < 256)
        minimum = 256; // See GetBufferTwo test

      if(minimum < (int32)_capacity * 2)
        minimum = (int32)_capacity * 2;

      return minimum;
      }

    void MemoryStream::Expand(int newSize)
      {
      // We don't need to take into account the dirty bytes when incrementing the
      // Capacity, as changing it will only preserve the valid clear region.
      if(newSize > (int)_capacity)
        Capacity(CalculateNewCapacity(newSize));
      else if(_dirty_bytes > 0) 
        {
        //TODO : Array.Clear(internalBuffer, length, dirty_bytes);
        _dirty_bytes = 0;
        }
      }

    void MemoryStream::WriteByte(byte value)
      {
      CheckIfClosedThrowDisposed();
      if(!_canWrite)
        //TODO : throw new NotSupportedException ("Cannot write to this stream.");
          throw SystemException(L"Cannot write to this stream.");

      if(_position >= (int32)_length)
        {
        Expand (_position + 1);
        _length = _position + 1;
        }

      (*_internalBuffer.Get())[_position++] = value;
      }

    void MemoryStream::Write(ByteArray&, int, int)
      {
      throw Exception(L"The method or operation is not implemented.");
      }

    }
  }
