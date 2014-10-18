#include "pch.h"
#include "System.Collections.BitArray.h"
#include "System.h"

namespace System
  {
  namespace Collections
    {

    BitArray::BitArray(int32 length)
      :_length(length)
      ,_version(0)
      ,_array(0)
      {
      if(_length < 0)
        throw ArgumentOutOfRangeException(L"length");

      if(_length > 0)
        _array.Length((_length + 31) / 32);
      }

    BitArray::BitArray(BoolArray& values)
      :_length(0)
      ,_version(0)
      ,_array(0)
      {
      if(values.IsNull())
        throw ArgumentNullException(L"values");

      _length = (int32)values.Length();
      _array.Length((_length + 31) / 32);

      for(int32 i = 0; i < (int32)values.Length(); i++)
        Set(i, values[i]);
      }

    BitArray::BitArray(ByteArray& bytes)
      :_length(0)
      ,_version(0)
      ,_array(0)
      {
      if(bytes.IsNull())
        throw ArgumentNullException (L"bytes");

      _length = (int32)bytes.Length() * 8;
      _array.Length((_length + 31) / 32);

      for(int i = 0; i < (int)bytes.Length(); i++)
        SetByte(i, bytes [i]);
      }

    BitArray::BitArray(IntArray& values)
      :_length(0)
      ,_version(0)
      ,_array(0)
      {
      if(values.IsNull())
        throw ArgumentNullException(L"values");

      int32 arrlen = (int32)values.Length();
      _length = arrlen*32;
      _array = values;
      }

    BitArray::BitArray(int32 length, bool defaultValue)
      :_length(length)
      ,_version(0)
      ,_array(0)
      {
      if(_length < 0)
        throw ArgumentOutOfRangeException(L"length");

      if(_length > 0)
        _array.Length((_length + 31) / 32);

      if(defaultValue) 
        {
        for(int32 i = 0; i < (int32)_array.Length(); i++)
          _array[i] = ~0;
        }
      else
        {
        for(int32 i = 0; i < (int32)_array.Length(); i++)
          _array[i] = 0;
        }
      }

    BitArray::BitArray(const BitArray& bitArray)
      :_length(bitArray._length)
      ,_version(bitArray._version)
      ,_array(bitArray._array)
      {
      }

    BitArray::~BitArray()
      {
      }

    BitArray& BitArray::operator=(const BitArray& bitArray)
      {
      if(this == &bitArray)
        return *this;
      _length = bitArray._length;
      _version = bitArray._version;
      _array = bitArray._array;
      return *this;
      }

    bool BitArray::IsReadOnly()
      {
      return false;
      }

    int32 BitArray::Length()
      {
      return _length;
      }

    void BitArray::Length(int32 value)
      {
      if(_length == value)
        return;

      if(value < 0)
        throw ArgumentOutOfRangeException();

      _array.Length(value);

      // set the internal state
      _length = value;
      _version++;
      }

    int32 BitArray::Count()
      {
      return _length;
      }

    bool BitArray::IsSynchronized()
      {
      return false;
      }

    IEnumerator* BitArray::GetEnumerator()
      {
      return nullptr;
      }

    bool BitArray::Get(int32 index)
      {
      if(index < 0 || index >= _length)
        throw ArgumentOutOfRangeException();

      return (_array[index >> 5] & (1 << (index & 31))) != 0;
      }

    void BitArray::Set(int32 index, bool value)
      {
      if(index < 0 || index >= _length)
        throw new ArgumentOutOfRangeException ();

      if(value)
        _array[index >> 5] |=  (1 << (index & 31));
      else
        _array[index >> 5] &= ~(1 << (index & 31));

      _version++;
      }

    BitArray& BitArray::And(BitArray& value)
      {
      CheckOperand(value);

      int32 ints = (_length + 31) / 32;
      for(int32 i = 0; i < ints; i++)
        _array[i] &= value._array[i];

      _version++;
      return *this;
      }

    BitArray& BitArray::Or(BitArray& value)
      {
      CheckOperand(value);

      int32 ints = (_length + 31) / 32;
      for(int32 i = 0; i < ints; i++)
        _array[i] |= value._array[i];

      _version++;
      return *this;
      }

    BitArray& BitArray::Not()
      {
      int32 ints = (_length + 31) / 32;
      for(int32 i = 0; i < ints; i++)
        _array[i] = ~_array[i];

      _version++;
      return *this;
      }

    BitArray& BitArray::Xor(BitArray& value)
      {
      CheckOperand(value);

      int32 ints = (_length + 31) / 32;
      for(int32 i = 0; i < ints; i++)
        _array[i] ^= value._array[i];

      _version++;
      return *this;
      }

    void BitArray::SetAll(bool value)
      {
      if(value)
        {
        for(int32 i = 0; i < (int32)_array.Length(); i++)
          _array[i] = ~0;
        }
      else
        {
        for(int32 i = 0; i < (int32)_array.Length(); i++)
          _array[i] = 0;
        }

      _version++;
      }

    void BitArray::CheckOperand(BitArray& operand)
      {
      if(&operand == nullptr)
        throw ArgumentNullException();
      if(operand._length != _length)
        throw ArgumentException ();
      }

    byte BitArray::GetByte(int32 byteIndex)
		{
			int32 index = byteIndex / 4;
			int32 shift = (byteIndex % 4) * 8;
			
			int32 theByte = _array[index] & (0xff << shift);
			
			return (byte)((theByte >> shift) & 0xff);
		}

    void BitArray::SetByte(int32 byteIndex, byte value)
      {
      int32 index = byteIndex / 4;
      int32 shift = (byteIndex % 4) * 8;

      // clear the byte
      _array[index] &= ~(0xff << shift);
      // or in the new byte
      _array[index] |= value << shift;

      _version++;
      }
    }
  }
