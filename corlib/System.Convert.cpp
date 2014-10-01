// Perch Lake Computer System

#include "pch.h"
#include "System.Convert.h"
#include "System.Array.hpp"
#include "System.Text.ASCIIEncoding.h"
#include "System.Security.Cryptography.Base64Helper.h"

namespace System
  {
  // Default constructor  
  Convert::Convert()
    {
    }
  // Destructor
  Convert::~Convert()
    {
    }
  ByteArray Convert::FromBase64CharArray(CharArray& inArray, int offset, int length)
    {
    if(inArray.IsNull())
      throw ArgumentNullException(L"inArray");
    if(offset < 0)
      throw ArgumentOutOfRangeException(L"offset < 0");
    if(length < 0)
      throw ArgumentOutOfRangeException(L"length < 0");
    // avoid integer overflow
    if(offset > (int)inArray.Length() - length)
      throw ArgumentOutOfRangeException(L"offset + length > array.Length");

    return InternalFromBase64CharArray(inArray, offset, length);
    }
  int Convert::ToBase64CharArray(ByteArray& inArray, int offsetIn, int length, CharArray& outArray, int offsetOut)
    {
    using namespace Security;
    if(inArray.IsNull())
      throw ArgumentNullException(L"inArray");
    if (outArray.IsNull())
      throw ArgumentNullException(L"outArray");
    if(offsetIn < 0 || length < 0 || offsetOut < 0)
      throw ArgumentOutOfRangeException(L"offsetIn, length, offsetOut < 0");
    // avoid integer overflow
    if(offsetIn > (int)inArray.Length() - length)
      throw ArgumentOutOfRangeException(L"offsetIn + length > array.Length");

    // note: normally ToBase64Transform doesn't support multiple block processing
    ByteArray outArr = Cryptography::Base64Helper::TransformFinalBlock(inArray, offsetIn, length);

    Text::ASCIIEncoding enc;    
    CharArray cOutArr = enc.GetChars(outArr);

    // avoid integer overflow
    if(offsetOut > (int)(outArray.Length() - cOutArr.Length()) )
      throw ArgumentOutOfRangeException(L"offsetOut + cOutArr.Length > outArray.Length");

    outArray.Base(offsetOut);
    for(sizet i = 0; i < cOutArr.Length(); ++i)
      outArray[i] = cOutArr[i];
    outArray.Base(0);

    return (int)cOutArr.Length();
    }
  ByteArray Convert::InternalFromBase64CharArray(CharArray& arr, int offset, int length)
    {
    using namespace Security;
    return Cryptography::Base64Helper::ToByteArray(arr.ToPtr() + offset, length, false);
    }
  byte Convert::ToByte(int value) 
		{ 
			return (byte)value; 
		}
  }
