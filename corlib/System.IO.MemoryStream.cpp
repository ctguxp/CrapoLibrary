#include "pch.h"
#include "System.IO.MemoryStream.h"
#include "System.h"
#include "System.IO.IOException.h"

namespace System
  {
  namespace IO
    {
    MemoryStream::MemoryStream(int32 capacity)
      :_canWrite(true)
      ,_expandable(true)
      ,_streamClosed(false)
      ,_allowGetBuffer(true) 
      ,_initialIndex(0)
      ,_position(0)
      ,_dirty_bytes(0)
      ,_capacity(capacity)
      ,_length(0)
      ,_internalBuffer()
      {
      if(_capacity < 0)
        {
        throw ArgumentOutOfRangeException(L"capacity");
        }
      _internalBuffer.Reset(new ByteArray(capacity));
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

    MemoryStream::MemoryStream(const SharedPtr<ByteArray>& buffer, int32 index, int32 count)
      :_streamClosed(false)
      ,_dirty_bytes(0)
      {
      InternalConstructor(buffer, index, count, true, false);
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

    int32 MemoryStream::Capacity() 
      { 
      CheckIfClosedThrowDisposed();
      return _capacity - _initialIndex;
      }
    void MemoryStream::Capacity(int32 value)
      { 
      CheckIfClosedThrowDisposed();

      if(!_expandable)
        throw NotSupportedException(L"Cannot expand this MemoryStream");

      if(value < 0 || value < _length)
        throw ArgumentOutOfRangeException(L"value", L"New capacity cannot be negative or less than the current capacity" /*+ value + " " + capacity*/);

      if(!_internalBuffer->IsNull() && value == (int32)_internalBuffer->Length())
        return;

      _internalBuffer->Length(value);
      _dirty_bytes = 0;
      _capacity = value;
      }

    int64 MemoryStream::Length()
      {
      CheckIfClosedThrowDisposed();
      return (int64)(_length - _initialIndex);
      }

    ByteArray& MemoryStream::GetBuffer()
      {
      if(!_allowGetBuffer)
        throw UnauthorizedAccessException();

      return (*_internalBuffer);
      }

    int32 MemoryStream::ReadByte() 
      {
      CheckIfClosedThrowDisposed();
      if(_position >= _length)
        return -1;

      return (*_internalBuffer)[_position++];
      }

    int64 MemoryStream::Seek(int64 offset, SeekOrigin loc)
      {
      CheckIfClosedThrowDisposed ();

      if(offset > (int64)Int32::MaxValue)
        throw ArgumentOutOfRangeException(L"Offset out of range. " + offset);

      int32 refPoint;
      switch(loc)
        {
        case SeekOrigin::Begin:
          if(offset < 0)
            throw new IOException(L"Attempted to seek before start of MemoryStream.");
          refPoint = _initialIndex;
          break;
        case SeekOrigin::Current:
          refPoint = _position;
          break;
        case SeekOrigin::End:
          refPoint = _length;
          break;
        default:
          throw new ArgumentException(L"loc", L"Invalid SeekOrigin");
        }

      refPoint += (int32)offset;
      if(refPoint < _initialIndex)
        throw IOException(L"Attempted to seek before start of MemoryStream.");

      _position = refPoint;
      return _position;
      }

    int64 MemoryStream::Position()
      {
      CheckIfClosedThrowDisposed ();
      return _position - _initialIndex;
      }

    void MemoryStream::Position(int64 value)
      {
      CheckIfClosedThrowDisposed ();

      if(value < 0)
        throw ArgumentOutOfRangeException (L"value", L"Position cannot be negative" );

      if(value > Int32::MaxValue)
        throw ArgumentOutOfRangeException(L"value", L"Position must be non-negative and less than 2^31 - 1 - origin");

      _position = _initialIndex + (int32)value;
      }

    bool MemoryStream::CanRead()
      {
      return !_streamClosed;
      }

    bool MemoryStream::CanSeek()
      {
      return !_streamClosed;
      }

    bool MemoryStream::CanWrite()
      {
      return (!_streamClosed && _canWrite);
      }

    void MemoryStream::Flush()
      {
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

      if(_position >= _length || count == 0)
        return 0;

      if(_position > _length - count)
        count = _length - _position;

      Buffer::BlockCopy((*_internalBuffer), _position, buffer, offset, count);

      _position += count;
      return count;
      }

    void MemoryStream::SetLength(int64 value)
      {
      if(!_expandable && value > _capacity)
        throw NotSupportedException(L"Expanding this MemoryStream is not supported");

      CheckIfClosedThrowDisposed ();

      if(!_canWrite) 
        {
        throw NotSupportedException (L"Cannot write to this MemoryStream");
        }

      // LAMESPEC: AGAIN! It says to throw this exception if value is
      // greater than "the maximum length of the MemoryStream".  I haven't
      // seen anywhere mention what the maximum length of a MemoryStream is and
      // since we're this far this memory stream is expandable.
      if (value < 0 || (value + _initialIndex) > (int64)Int32::MaxValue)
        throw new ArgumentOutOfRangeException ();

      int32 newSize = (int32)value + _initialIndex;

      if(newSize > _length)
        Expand(newSize);
      else if(newSize < _length) // Postpone the call to Array.Clear till expand time
        _dirty_bytes += _length - newSize;

      _length = newSize;
      if(_position > _length)
        _position = _length;
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
        throw NotSupportedException(L"Cannot write to this stream.");

      if(_position >= _length)
        {
        Expand(_position + 1);
        _length = _position + 1;
        }

      (*_internalBuffer)[_position++] = value;
      }

    void MemoryStream::Write(ByteArray& buffer, int32 offset, int32 count)
      {
      if(buffer.IsNull())
        throw ArgumentNullException(L"buffer");

      if (offset < 0 || count < 0)
        throw ArgumentOutOfRangeException ();

      if((int32)buffer.Length() - offset < count)
        throw ArgumentException(L"offset+count", L"The size of the buffer is less than offset + count.");

      CheckIfClosedThrowDisposed ();

      if(!CanWrite())
        throw NotSupportedException(L"Cannot write to this stream.");

      // reordered to avoid possible integer overflow
      if(_position > _length - count)
        Expand(_position + count);

      Buffer::BlockCopy(buffer, offset, (*_internalBuffer), _position, count);
      _position += count;
      if(_position >= _length)
        _length = _position;
      }

    ByteArray MemoryStream::ToArray()
      { 
      int32 l = _length - _initialIndex;
      ByteArray outBuffer(l);

      if(_internalBuffer.Get() != nullptr)
        Buffer::BlockCopy((*_internalBuffer), _initialIndex, outBuffer, 0, l);
      return outBuffer;
      }
    }
  }
