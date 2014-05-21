#include "pch.h"
#include "Crapo.Security.BitConverterLE.h"
#include "System.h"

using namespace System;

namespace Crapo
  {
  namespace Security
    {
    BitConverterLE::BitConverterLE()
      {
      }
    BitConverterLE::~BitConverterLE()
      {
      }
    ByteArray BitConverterLE::GetBytes(float value)
      {
      return GetUIntBytes((byte*)&value);
      }
    ByteArray BitConverterLE::GetUIntBytes(byte *bytes)
      {
      if(BitConverter::IsLittleEndian)
        {
        byte temp[4] = { bytes [0], bytes [1], bytes [2], bytes [3] };
        return ByteArray((const byte*)temp, 4);
        }
      else
        {
        byte temp[4] = { bytes [3], bytes [2], bytes [1], bytes [0] };
        return ByteArray((const byte*)temp, 4);
        }
      }
    void BitConverterLE::UIntFromBytes(byte* dst, ByteArray& src, int startIndex)
      {
      if (BitConverter::IsLittleEndian)
        {
        dst [0] = src[startIndex];
        dst [1] = src[startIndex + 1];
        dst [2] = src[startIndex + 2];
        dst [3] = src[startIndex + 3];
        }
      else
        {
        dst [0] = src[startIndex + 3];
        dst [1] = src[startIndex + 2];
        dst [2] = src[startIndex + 1];
        dst [3] = src[startIndex];
        }
      }
    float BitConverterLE::ToSingle(ByteArray& value, int startIndex)
      {
      float ret;

      UIntFromBytes((byte*) &ret, value, startIndex);

      return ret;
      }
    }
  }
