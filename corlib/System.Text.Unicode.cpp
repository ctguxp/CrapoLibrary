#include "pch.h"
#include "System.Text.Unicode.h"
#include "System.BitConverter.h"

namespace System
  {
  namespace Text
    {
    // Unicode encoding
    // Default constructor
    UnicodeEncoding::UnicodeEncoding(bool bigEndian, bool byteOrderMark, bool throwOnInvalidBytes)
      :Encoding(bigEndian ? BIG_UNICODE_CODE_PAGE : UNICODE_CODE_PAGE)
      ,_isThrowException(throwOnInvalidBytes)
      ,_bigEndian(bigEndian)
      ,_byteOrderMark(byteOrderMark)
      {
      if(_isThrowException)
        {
        // TODO : Add function SetDefaultFallbacks ?
        }
      // TODO : BigEndian naming
      }
    // Destructor
    UnicodeEncoding::~UnicodeEncoding()
      {
      }

    // Returns a Unicode byte order mark encoded in UTF-16 format, if the constructor for this instance requests a byte order mark.
    ByteArray UnicodeEncoding::GetPreamble()
      {
      if(!_byteOrderMark)
        return System::ByteArray();
      if(_bigEndian)
        {
        ByteArray retval(2);
        retval.operator[](0) = 0xFE;
        retval.operator[](1) = 0xFF;
        return retval;
        }
      ByteArray retval(2);
      retval.operator[](0) = 0xFF;
      retval.operator[](1) = 0xFE;
      return retval;
      }

    int UnicodeEncoding::GetByteCount(CharArray&, int, int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetByteCount");
      }

    int UnicodeEncoding::GetByteCount(String&)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetByteCount");
      }

    int UnicodeEncoding::GetByteCount(CharArray&)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetByteCount");
      }

    int UnicodeEncoding::GetByteCount(wchar_t*, int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetByteCount");
      }

    int UnicodeEncoding::GetBytes(System::CharArray&, int, int, System::ByteArray&, int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetBytes");
      }

    int UnicodeEncoding::GetBytes(System::String&, int, int, System::ByteArray&, int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetBytes");
      }

    int UnicodeEncoding::GetBytes(wchar_t*, int, byte*, int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetBytes");
      }

    ByteArray UnicodeEncoding::GetBytes(String&)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetBytes");
      }

    ByteArray UnicodeEncoding::GetBytes(CharArray&, int, int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetBytes");
      }

    ByteArray UnicodeEncoding::GetBytes(CharArray&)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetBytes");
      }

    int UnicodeEncoding::GetCharCount(ByteArray&, int, int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetCharCount");
      }

    int UnicodeEncoding::GetCharCount(ByteArray&)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetCharCount");
      }

    int UnicodeEncoding::GetChars(ByteArray& bytes, int byteIndex, int byteCount, CharArray& chars, int charIndex)
      {
      if(bytes.IsNull()) 
        {
        throw ArgumentNullException(L"bytes");
        }
      if(chars.IsNull())
        {
        throw ArgumentNullException(L"chars");
        }
      if(byteIndex < 0 || byteIndex > (int)bytes.Length()) 
        {
        throw ArgumentOutOfRangeException(L"byteIndex", L"ArgRange_Array");
        }
      if(byteCount < 0 || byteCount > ((int)bytes.Length() - byteIndex)) 
        {
        throw ArgumentOutOfRangeException(L"byteCount", L"ArgRange_Array");
        }
      if(charIndex < 0 || charIndex > (int)chars.Length()) 
        {
        throw ArgumentOutOfRangeException(L"charIndex", L"ArgRange_Array");
        }

      if(byteCount == 0)
        return 0;

      int charCount = (int)chars.Length() - charIndex;
      if(chars.Length() == 0)
        chars.Length(1);

      byte* bytePtr = bytes.ToPtr();
      string charPtr = chars.ToPtr();
      return GetCharsInternal(bytePtr + byteIndex, byteCount, charPtr + charIndex, charCount);
      }

    CharArray UnicodeEncoding::GetChars(ByteArray& bytes)
      {
      return Encoding::GetChars(bytes);
      }

    CharArray UnicodeEncoding::GetChars(ByteArray& bytes, int index, int count)
      {
      return Encoding::GetChars(bytes, index, count);
      }

    int UnicodeEncoding::GetChars(byte*, int, wchar_t*, int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetChars");
      }

    int UnicodeEncoding::GetCharsInternal(byte* bytes, int byteCount, string chars, int charCount)
      {
      int count = byteCount / 2;

      // Validate that we have sufficient space in "chars".
      if (charCount < count)
        throw ArgumentException(L"Arg_InsufficientSpace");

      CopyChars(bytes, (byte*)chars, byteCount, _bigEndian);
      return count;
      }

    int UnicodeEncoding::GetMaxByteCount(int)
      {
      throw ArgumentException(L"Not Implemented", L"UnicodeEncoding::GetMaxByteCount");
      }

    int UnicodeEncoding::GetMaxCharCount(int byteCount)
      {
      if(byteCount < 0)
        throw ArgumentOutOfRangeException(L"byteCount", L"ArgRange_NonNegative");
      return byteCount / 2;
      }

    int UnicodeEncoding::GetCharCount (byte* bytes, int count)
      {
      if(bytes == nullptr)
        throw ArgumentNullException(L"bytes");
      if(count < 0)
        throw ArgumentOutOfRangeException(L"count");

      return count / 2;
      }

    void UnicodeEncoding::CopyChars(byte* src, byte* dest, int count, bool bigEndian)
      {
      if(BitConverter::IsLittleEndian != bigEndian) 
        {
        String::memcpy(dest, src, count & ((int)0xFFFFFFFE));
        return;
        }

      switch(count)
        {
        case 0:
          return;
        case 1:
          return;
        case 2:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          return;
          }
        case 3:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          return;
          }
        case 4:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest += 4;
          src += 4;
          return;
          }
        case 5:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest += 4;
          src += 4;
          return;
          }
        case 6:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest += 4;
          src += 4;

          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        case 7:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest += 4;
          src += 4;

          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        case 8:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest [4] = src [5];
          dest [5] = src [4];
          dest [6] = src [7];
          dest [7] = src [6];
          dest += 8;
          src += 8;
          return;
          }
        case 9:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest [4] = src [5];
          dest [5] = src [4];
          dest [6] = src [7];
          dest [7] = src [6];
          dest += 8;
          src += 8;
          return;
          }
        case 10:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest [4] = src [5];
          dest [5] = src [4];
          dest [6] = src [7];
          dest [7] = src [6];
          dest += 8;
          src += 8;

          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        case 11:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest [4] = src [5];
          dest [5] = src [4];
          dest [6] = src [7];
          dest [7] = src [6];
          dest += 8;
          src += 8;

          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        case 12:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest [4] = src [5];
          dest [5] = src [4];
          dest [6] = src [7];
          dest [7] = src [6];
          dest += 8;
          src += 8;

          dest[0] = src[1];
          dest[1] = src[0];
          dest[2] = src[3];
          dest[3] = src[2];
          dest += 4;
          src += 4;
          return;
          }
        case 13:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest [4] = src [5];
          dest [5] = src [4];
          dest [6] = src [7];
          dest [7] = src [6];
          dest += 8;
          src += 8;

          dest[0] = src[1];
          dest[1] = src[0];
          dest[2] = src[3];
          dest[3] = src[2];
          dest += 4;
          src += 4;
          return;
          }
        case 14:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest [4] = src [5];
          dest [5] = src [4];
          dest [6] = src [7];
          dest [7] = src [6];
          dest += 8;
          src += 8;

          dest[0] = src[1];
          dest[1] = src[0];
          dest[2] = src[3];
          dest[3] = src[2];
          dest += 4;
          src += 4;

          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        case 15:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest [4] = src [5];
          dest [5] = src [4];
          dest [6] = src [7];
          dest [7] = src [6];
          dest += 8;
          src += 8;

          dest[0] = src[1];
          dest[1] = src[0];
          dest[2] = src[3];
          dest[3] = src[2];
          dest += 4;
          src += 4;

          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        }

      do {
        dest [0] = src [1];
        dest [1] = src [0];
        dest [2] = src [3];
        dest [3] = src [2];
        dest [4] = src [5];
        dest [5] = src [4];
        dest [6] = src [7];
        dest [7] = src [6];
        dest [8] = src [9];
        dest [9] = src [8];
        dest [10] = src [11];
        dest [11] = src [10];
        dest [12] = src [13];
        dest [13] = src [12];
        dest [14] = src [15];
        dest [15] = src [14];
        dest += 16;
        src += 16;
        count -= 16;
        } while ((count & ((int) 0xFFFFFFF0)) != 0);

      switch (count) 
        {
        case 0:
          return;
        case 1:
          return;
        case 2:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          return;
          }
        case 3:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          return;
          }
        case 4:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest += 4;
          src += 4;
          return;
          }
        case 5:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest += 4;
          src += 4;
          return;
          }
        case 6:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest += 4;
          src += 4;

          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        case 7:
          {
          dest [0] = src [1];
          dest [1] = src [0];
          dest [2] = src [3];
          dest [3] = src [2];
          dest += 4;
          src += 4;

          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        }

      dest[0] = src[1];
      dest[1] = src[0];
      dest[2] = src[3];
      dest[3] = src[2];
      dest[4] = src[5];
      dest[5] = src[4];
      dest[6] = src[7];
      dest[7] = src[6];
      dest += 8;
      src += 8;

      if ((count & 4) == 0)
        {
        if ((count & 2) == 0)
          return;
        else
          {
          dest[0] = src[1];
          dest[1] = src[0];
          return;
          }
        }
      else
        {
        dest[0] = src[1];
        dest[1] = src[0];
        dest[2] = src[3];
        dest[3] = src[2];
        dest += 4;
        src += 4;

        if ((count & 2) == 0)
          return;
        }

      dest[0] = src[1];
      dest[1] = src[0];
      }
    }
  }