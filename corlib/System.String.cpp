// Perch Lake Computer System

#include "pch.h"
#include "System.String.h"
#include "System.Char.h"
#include "System.Text.StringBuilder.h"
#include "System.OutOfMemoryException.h"
#include "System.Text.UTF8Encoding.h"
#include "System.Int32.h"
#include "System.Math.h"
#include "System.Globalization.CultureInfo.h"
#include "System.Collections.Generic.List.hpp"

uint32 const shift = 6;
uint32 const mask = ~0U << ((sizeof(uint32) * 8) - 6);

namespace System
  {
  /// Constructors
  // ------------------------------------------------------------------------
  /// Initializes a new instance of the String class to the value indicated by a specified pointer to an array of Unicode characters.
  String::String(const wchar_t* value)
    :_length( value != nullptr ? (uint32)WcsLen(value) : 0)
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    if(_length != 0)
      {
      CharCopy(_buffer, value, _length);
      _buffer[_length] = L'\0';
      }
    else
      _buffer[0] = L'\0';
    }
  // ------------------------------------------------------------------------

  String::String(wchar_t c)
    :_length(1)
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    _buffer[0] = c;
    _buffer[_length] = L'\0';
    }

  // ------------------------------------------------------------------------
  /// Initializes a new instance of the String class to the value indicated by a specified pointer to an array of Unicode characters, a starting character position within that array, and a length.
  String::String(const wchar_t* s, uint32 startIndex, uint32 length)
    :_length(length)
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    if(length != 0)
      {
      CharCopy(_buffer, s + startIndex, length);
      _buffer[_length] = L'\0';
      }
    else
      _buffer[0] = L'\0';
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Initializes a new instance of the String class to the value indicated by a pointer to an array of 8-bit signed integers.
  String::String(sbyte* value)
    :_length( value != nullptr ? (uint32)StrLen(value) : 0 )
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    if(_length != 0)
      {
      *this = CreateString(value, 0, _length, nullptr);
      }
    else
      _buffer[0] = L'\0';
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Initializes a new instance of the String class to the value indicated by a specified pointer to an array of 8-bit signed 
  /// integers,a starting position within that array, and a length.
  String::String(sbyte* value, uint32 startIndex, uint32 length)
    :_length(length)
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    if(length != 0)
      {
      *this = CreateString(value, startIndex, length, nullptr);
      }
    else
      _buffer[0] = L'\0';
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Initializes a new instance of the String class to the value indicated by a specified pointer to an array of 8-bit signed 
  /// integers, a starting position within that array, a length, and an Encoding object.
  String::String(sbyte* value, uint32 startIndex, uint32 length, Text::Encoding* enc)
    :_length(length)
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    if(length != 0)
      {
      *this = CreateString(value, startIndex, length, enc);
      }
    else
      _buffer[0] = L'\0';
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Initializes a new instance of the String class to the value indicated by an array of Unicode characters.
  String::String(const CharArray& val)
    :_length((uint32)val.Length())
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    if(!val.IsNull() || val.Length() != 0)
      {
      cstring src = val.ToConstPtr();
      CharCopy(_start_char, src, (int)val.Length());
      _buffer[_length] = L'\0';
      }
    else
      {
      _buffer[0] = L'\0';
      }
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Initializes a new instance of the String class to the value indicated by a specified Unicode character repeated a specified number of times.
  String::String(wchar_t c, uint32 count)
    :_length(count)
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    if(_length != 0)
      {
      wchar_t* p = _start_char;
      wchar_t* end = _start_char + count;
      while(p < end)
        {
        *p = c;
        ++p;
        }
      _buffer[_length] = L'\0';
      }
    else
      _buffer[0] = L'\0';
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Copy constructor
  String::String(const String& s)
    :_length(s._length)
    ,_buffer(new wchar_t[_length + 1])
    ,_start_char(_buffer)
    {
    if(_length != 0)
      {
      CharCopy(_buffer, s._buffer, _length);
      _buffer[_length] = L'\0';
      }
    else
      _buffer[0] = L'\0';
    }
  // ------------------------------------------------------------------------


  /// Destructor & Operators
  // ------------------------------------------------------------------------
  /// Copy constructor
  String::~String()
    {
    free();
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Assignment operator
  String& String::operator = (const String& s)
    {
    if(this == &s)
      return *this;

    _length = s._length;
    free();
    _buffer = new wchar_t[_length + 1];
    _start_char = _buffer;

    if(_length != 0)
      {
      CharCopy(_buffer, s._buffer, _length);
      _buffer[_length] = L'\0';
      }
    else
      _buffer[0] = L'\0';

    return *this;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Compound assignment operator
  String& String::operator += (const String& s)
    {
    Text::StringBuilder sb(*this);
    sb.Append(s);
    *this = sb.ToString();
    return *this;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Compound assignment operator
  String& String::operator += (const wchar_t& c)
    {
    String cs(c);
    Text::StringBuilder sb(*this);
    sb.Append(cs);
    *this = sb.ToString();
    return *this;
    }
  // ------------------------------------------------------------------------


  /// Fields
  // ------------------------------------------------------------------------
  /// Represents the empty string
  String String::Empty()
    {
    return String();
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Compares this instance with a specified String object and indicates whether this instance precedes, follows, or 
  /// appears in the same position in the sort order as the specified String.
  int String::CompareTo(String& value)
    {
    return String::Compare(*this, value);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Compares two specified String objects and returns an integer that indicates their relative position in the sort order.
  int String::Compare(String& strA, String& strB)
    {
    using namespace Globalization;
    return CultureInfo::CurrentCulture().CompareInfo().Compare(strA, strB, CompareOptions::None);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Compares substrings of two specified String objects by evaluating the numeric values of the corresponding Char objects in each substring.
  int String::CompareOrdinal(String& strA, int indexA, String& strB, int indexB, int length)
    {
    if (indexA > strA.Length() || indexA < 0)
      throw ArgumentOutOfRangeException(L"indexA");
    if (indexB > strB.Length() || indexB < 0)
      throw ArgumentOutOfRangeException(L"indexB");
    if(length < 0)
      throw ArgumentOutOfRangeException(L"length");

    return CompareOrdinalUnchecked(strA, indexA, length, strB, indexB, length);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Concatenates three specified instances of String.
  String String::Concat(String str0, String str1, String str2)
    {
    if(str0.Length() == 0)
      {
      if(str1.Length() == 0)
        {
        if (str2.Length() == 0)
          return Empty();
        return str2;
        } 
      else 
        {
        if(str2.Length() == 0)
          return str1;
        }
      str0 = Empty();
      } 
    else 
      {
      if(str1.Length() == 0)
        {
        if(str2.Length() == 0)
          return str0;
        else
          str1 = Empty();
        } 
      else
        {
        if(str2.Length() == 0)
          str2 = Empty();
        }
      }

    int nlen = str0.Length() + str1.Length();
    if (nlen < 0)
      throw OutOfMemoryException ();
    nlen += str2.Length();
    if (nlen < 0)
      throw new OutOfMemoryException ();
    String tmp = InternalAllocateStr(nlen + 1);

    if(str0.Length() != 0) 
      {
      wchar_t* dest = tmp._start_char;
      wchar_t* src = str0._start_char;
        {
        CharCopy(dest, src, str0.Length());
        }
      }
    if(str1.Length() != 0) {
      wchar_t* dest = tmp._start_char;
      wchar_t* src = str1._start_char;
        {
        CharCopy(dest + str0.Length(), src, str1.Length());
        }
      }
    if(str2.Length() != 0) 
      {
      wchar_t* dest = tmp._start_char;
      wchar_t* src = str2._start_char;
        {
        CharCopy (dest + str0.Length() + str1.Length(), src, str2.Length());
        }
      }

    tmp.check_null();
    return tmp;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a value indicating whether a specified substring occurs within this string.
  bool String::Contains(const String& value)
    {
    return IndexOfOrdinalUnchecked(value, 0, Length()) != -1;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Determines whether the end of this string instance matches the specified string.
  bool String::EndsWith(String value)
    {
    using namespace Globalization;
    return CultureInfo::CurrentCulture().CompareInfo().IsSuffix(*this, value, CompareOptions::None);
    }
  // ------------------------------------------------------------------------

  bool String::Equals(Object* obj)
    {
    String temp = obj->ToString();
    return Equals(*this, temp);
    }


  // ------------------------------------------------------------------------
  /// Determines whether this instance and another specified String object have the same value.
  bool String::Equals(String other)
    {
    return String::Equals(*this, other);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Determines whether two specified String objects have the same value.
  bool String::Equals(const String& a, const String& b)
    {
    uint32 len = a._length;

    if(len != b._length)
      return false;

    string s1 = a._start_char;
    string  s2 = b._start_char; 
    string  s1_ptr = s1;
    string  s2_ptr = s2;

    while (len >= 8)
      {
      if (((int*)s1_ptr)[0] != ((int*)s2_ptr)[0] ||
        ((int*)s1_ptr)[1] != ((int*)s2_ptr)[1] ||
        ((int*)s1_ptr)[2] != ((int*)s2_ptr)[2] ||
        ((int*)s1_ptr)[3] != ((int*)s2_ptr)[3])
        return false;

      s1_ptr += 8;
      s2_ptr += 8;
      len -= 8;
      }

    if (len >= 4)
      {
      if (((int*)s1_ptr)[0] != ((int*)s2_ptr)[0] ||
        ((int*)s1_ptr)[1] != ((int*)s2_ptr)[1])
        return false;

      s1_ptr += 4;
      s2_ptr += 4;
      len -= 4;
      }

    if (len > 1)
      {
      if (((int*)s1_ptr)[0] != ((int*)s2_ptr)[0])
        return false;

      s1_ptr += 2;
      s2_ptr += 2;
      len -= 2;
      }

    return len == 0 || *s1_ptr == *s2_ptr;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Replaces one or more format items in a specified string with the string representation of a
  /// specified object.
  String String::Format(String format, Object* obj)
    {
    ObjectArray temp(1);
    temp.Add(0, *obj);
    temp.RescindOwnership();	
    Text::GCStringBuilder sb(FormatHelper(nullptr, format, temp));
    return sb->ToString();
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Replaces the format items in a specified string with the string representation
  /// of two specified objects.
  String String::Format(String format, Object* obj1, Object* obj2)
    {
    ObjectArray temp(2);
    temp.Add(0, *obj1);
    temp.Add(1, *obj2);
    temp.RescindOwnership();	
    Text::GCStringBuilder sb(FormatHelper(nullptr, format, temp));
    return sb->ToString();
    }
  // ------------------------------------------------------------------------

  String String::Format(String format, Object* obj1, Object* obj2, Object* obj3)
    {
    ObjectArray temp(3);
    temp.Add(0, *obj1);
    temp.Add(1, *obj2);
    temp.Add(2, *obj3);
    temp.RescindOwnership();	
    Text::GCStringBuilder sb(FormatHelper(nullptr, format, temp));
    return sb->ToString();
    }

  // ------------------------------------------------------------------------
  /// Reports the zero-based index of the first occurrence of the specified Unicode character in this string.
  int String::IndexOf(wchar_t value)
    {
    if(_length == 0)
      return -1;

    return IndexOfUnchecked (value, 0, _length);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Reports the zero-based index position of the last occurrence of a specified Unicode character within
  /// this instance. The search starts at a specified character position and proceeds backward toward the 
  /// beginning of the string.
  int String::LastIndexOf(wchar_t value, int startIndex)
    {
    return LastIndexOf(value, startIndex, startIndex + 1);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Reports the zero-based index position of the last occurrence of the specified Unicode character in
  /// a substring within this instance. The search starts at a specified character position and proceeds
  /// backward toward the beginning of the string for a specified number of character positions.
  int String::LastIndexOf(wchar_t value, int startIndex, int count)
    {
    if(_length == 0)
      return -1;

    // >= for char (> for string)
    if((startIndex < 0) || (startIndex >= Length()))
      throw ArgumentOutOfRangeException(L"startIndex", L"< 0 || >= this.Length");
    if((count < 0) || (count > Length()))
      throw ArgumentOutOfRangeException(L"count", L"< 0 || > this.Length");
    if(startIndex - count + 1 < 0)
      throw ArgumentOutOfRangeException(L"startIndex - count + 1 < 0");

    return LastIndexOfUnchecked(value, startIndex, count);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Reports the zero-based index of the first occurrence of the specified Unicode character in this string.
  /// The search starts at a specified character position.
  int String::IndexOf(wchar_t value, int startIndex)
    {
    if(startIndex < 0)
      throw ArgumentOutOfRangeException(L"startIndex", L"< 0");
    if(startIndex > (int)_length)
      throw ArgumentOutOfRangeException(L"startIndex", L"startIndex > this.length");

    if((startIndex == 0 && _length == 0) || (startIndex == (int)_length))
      return -1;

    return IndexOfUnchecked(value, startIndex, _length - startIndex);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Concatenates the specified elements of a string array, using the specified separator between each element.
  String String::Join(String separator, StringArray& value, int startIndex, int count)
    {
    if(value.IsNull())
      throw ArgumentNullException(L"value");
    if(startIndex < 0)
      throw ArgumentOutOfRangeException(L"startIndex", L"< 0");
    if(count < 0)
      throw ArgumentOutOfRangeException(L"count", L"< 0");
    if(startIndex > (int)value.Length() - count)
      throw ArgumentOutOfRangeException(L"startIndex", L"startIndex + count > value.length");

    if (startIndex == (int)value.Length())
      return Empty();

    return JoinUnchecked(separator, value, startIndex, count);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a new string in which all occurrences of a specified Unicode character in this
  /// instance are replaced with another specified Unicode character.
  String String::Replace(wchar_t oldChar, wchar_t newChar)
    {
    if(_length == 0 || oldChar == newChar)
      return *this;

    int start_pos = IndexOfUnchecked(oldChar, 0, _length);
    if(start_pos == -1)
      return *this;

    if(start_pos < 4)
      start_pos = 0;

    String tmp = InternalAllocateStr(_length + 1); // Internal shortens by one so add one
    wchar_t* dest = tmp._start_char;
    wchar_t* src = _start_char; 
      {
      if(start_pos != 0)
        CharCopy(dest, src, start_pos);

      wchar_t* end_ptr = dest + _length;
      wchar_t* dest_ptr = dest + start_pos;
      wchar_t* src_ptr = src + start_pos;

      while(dest_ptr != end_ptr) 
        {
        if(*src_ptr == oldChar)
          *dest_ptr = newChar;
        else
          *dest_ptr = *src_ptr;

        ++src_ptr;
        ++dest_ptr;
        }
      }
      return tmp;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a string array that contains the substrings in this instance that are delimited by elements
  /// of a specified Unicode character array.
  StringArray String::Split(CharArray& separator)
    {
    return Split(separator, Int32::MaxValue, StringSplitOptions::None);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a string array that contains the substrings in this instance that are delimited by elements
  /// of a specified Unicode character array. A parameter specifies the maximum number of substrings to 
  /// return.
  StringArray String::Split(CharArray& separator, StringSplitOptions options)
    {
    return Split(separator, Int32::MaxValue, options);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a string array that contains the substrings in this string that are delimited by elements
  /// of a specified Unicode character array. Parameters specify the maximum number of substrings to 
  /// return and whether to return empty array elements.
  StringArray String::Split(CharArray& separator, int count, StringSplitOptions options)
    {
    if(count < 0)
      throw ArgumentOutOfRangeException(L"count", L"Count cannot be less than zero.");
    if ((options != StringSplitOptions::None) && (options != StringSplitOptions::RemoveEmptyEntries))
      throw ArgumentException(L"Illegal enum value: ");
    // TODO: throw ArgumentException(L"Illegal enum value: " + options + ".");

    if(_length == 0 && ((ulong)options & (ulong)StringSplitOptions::RemoveEmptyEntries) != 0)
      return StringArray();

    if(count <= 1) 
      {
      if(count == 0)
        return StringArray();
      else
        {
        StringArray ret(1);
        ret[0] = *this;
        return ret;
        }
      }

    return SplitByCharacters(separator, count, (ulong)options != 0);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a string array that contains the substrings in this string that are delimited by elements
  /// of a specified string array. A parameter specifies whether to return empty array elements.
  StringArray String::Split(StringArray& separator, StringSplitOptions options)
    {
    return Split(separator, Int32::MaxValue, options);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a string array that contains the substrings in this string that are delimited by elements
  /// of a specified string array. Parameters specify the maximum number of substrings to return and
  /// whether to return empty array elements.
  StringArray String::Split(StringArray& separator, int count, StringSplitOptions options)
    {
    using namespace Collections;
    if(count < 0)
      throw ArgumentOutOfRangeException(L"count", L"Count cannot be less than zero.");
    if ((options != StringSplitOptions::None) && (options != StringSplitOptions::RemoveEmptyEntries))
      throw ArgumentException(L"Illegal enum value: ");
    //throw new ArgumentException(L"Illegal enum value: " + options + ".");

    if(count <= 1) 
      {
      if(count != 0)
        {
        StringArray ret(1);
        ret[0] = *this;
        return ret;
        }
      return StringArray();
      }

    bool removeEmpty = ((ulong)options & (ulong)StringSplitOptions::RemoveEmptyEntries) != 0;

    if(separator.IsNull() || separator.Length() == 0)
      {
      CharArray n;
      return SplitByCharacters(n, count, removeEmpty);
      }

    if(Length() == 0 && removeEmpty)
      return StringArray();

    Generic::List<String> arr;

    int pos = 0;
    int matchCount = 0;
    while(pos < Length())
      {
      int matchIndex = -1;
      int matchPos = Int32::MaxValue;

      // Find the first position where any of the separators matches
      for(int i = 0; i < (int)separator.Length(); ++i)
        {
        String sep = separator[i];
        if(sep.Length() == 0)
          continue;

        int match = IndexOfOrdinalUnchecked(sep, pos, _length - pos);
        if(match >= 0 && match < matchPos)
          {
          matchIndex = i;
          matchPos = match;
          }
        }

      if(matchIndex == -1)
        break;

      if(!(matchPos == pos && removeEmpty)) 
        {
        if((int)arr.Count() == count - 1)
          break;
        arr.Add(Substring(pos, matchPos - pos));
        }

      pos = matchPos + separator[matchIndex].Length();

      matchCount ++;
      }

    if(matchCount == 0)
      {
      StringArray ret(1);
      ret[0] = *this;
      return ret;
      }

    // string contained only separators
    if(removeEmpty && matchCount != 0 && pos == Length() && arr.Count() == 0)
      return StringArray();

    if(!(removeEmpty && pos == Length()))
      arr.Add(Substring(pos));

    return arr.ToArray();
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Substring specified pos to end of string
  String String::Substring(uint32 startIndex)
    {
    if(startIndex == 0)
      return *this;
    if(startIndex > _length)
      throw ArgumentOutOfRangeException(L"startIndex");

    return SubstringUnchecked(startIndex, _length - startIndex);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  // Substring specified pos and has specified length
  String String::Substring(uint32 startIndex, uint32 length)
    {
    if(startIndex > _length)
      throw ArgumentOutOfRangeException(L"startIndex", L"Cannot exceed length of string.");
    if(startIndex > _length - length)
      throw ArgumentOutOfRangeException(L"length", L"startIndex + length cannot exceed length of string.");
    if(startIndex == 0 && length == _length)
      return *this;

    return SubstringUnchecked(startIndex, length);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Copies the characters in this instance to a Unicode character array.
  CharArray String::ToCharArray()
    {
    return ToCharArray(0, _length);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Copies the characters in a specified substring in this instance to a Unicode character array.
  CharArray String::ToCharArray(uint32 startIndex, uint32 length)
    {
    if (startIndex > _length - length)
      throw ArgumentOutOfRangeException(L"startIndex", L"Must be greater than the length of the string.");

    CharArray tmp((wchar_t*)_start_char + startIndex, length);
    return tmp;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a copy of this string converted to lowercase.
  String String::ToLower()
    {
    return ToLower(&Globalization::CultureInfo::CurrentCulture());
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Returns a copy of this string converted to lowercase, using the casing rules of the specified culture.
  String String::ToLower(Globalization::CultureInfo* culture)
		{
			if(culture == nullptr)
				throw ArgumentNullException(L"culture");

			if(culture->LCID() == 0x007F) // Invariant
				return ToLowerInvariant();

			return culture->TextInfo().ToLower(*this);
		}
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Removes all leading and trailing white-space characters from the current String object.
  String String::Trim()
    {
    if(_length == 0) 
      return Empty();
    int start = FindNotWhiteSpace(0, _length, 1);

    if(start == (int)_length)
      return Empty();

    int end = FindNotWhiteSpace(_length - 1, start, -1);

    int newLength = end - start + 1;
    if(newLength == (int)_length)
      return *this;

    return SubstringUnchecked(start, newLength);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// Removes all trailing occurrences of a set of characters specified in an array from the current String object.
  String String::TrimEnd()
    {
    CharArray empty;
    return TrimEnd(empty);
    }
  String String::TrimEnd(CharArray& trimChars)
    {
    if(_length == 0) 
      return Empty();
    int end;
    if(trimChars.IsNull() || trimChars.Length() == 0)
      end = FindNotWhiteSpace(_length - 1, -1, -1);
    else
      end = FindNotInTable(_length - 1, -1, -1, trimChars);

    end++;
    if(end == (int)_length)
      return *this;

    return SubstringUnchecked(0, end);
    }
  // ------------------------------------------------------------------------


  /// Properties
  // ------------------------------------------------------------------------
  /// Gets the number of characters in the current String object.
  int String::Length()
    {
    return (int)_length;
    }
  int String::Length() const
    {
    return (int)_length;
    }
  // ------------------------------------------------------------------------


  /// Internal

  //int String::Compare(String& strA, int indexA, String& strB, int indexB, int length, bool ignoreCase, Globalization::CultureInfo* culture)
  //  {
  //  if(culture == nullptr)
  //    throw ArgumentNullException(L"culture");

  //  if((indexA > (int)strA.Length()) || (indexB > (int)strB.Length()) || (indexA < 0) || (indexB < 0) || (length < 0))
  //    throw new ArgumentOutOfRangeException ();

  //  if(length == 0)
  //    return 0;

  //  Globalization::CompareOptions compopts;

  //  if(ignoreCase)
  //    compopts = Globalization::CompareOptions::IgnoreCase;
  //  else
  //    compopts = Globalization::CompareOptions::None;

  //  // Need to cap the requested length to the
  //  // length of the string, because
  //  // CompareInfo.Compare will insist that length
  //  // <= (string.Length - offset)

  //  int len1 = length;
  //  int len2 = length;

  //  if(length > ((int)strA.Length() - indexA)) 
  //    {
  //    len1 = strA.Length() - indexA;
  //    }

  //  if (length > ((int)strB.Length() - indexB))
  //    {
  //    len2 = strB.Length() - indexB;
  //    }

  //  // ENHANCE: Might call internal_compare_switch directly instead of doing all checks twice
  //  return culture->CompareInfo().Compare(strA, indexA, len1, strB, indexB, len2, compopts);
  //  }

  // ------------------------------------------------------------------------
  /// CompareOrdinalUnchecked function
  int String::CompareOrdinalUnchecked(String& strA, int indexA, int lenA, String& strB, int indexB, int lenB)
    {
    int lengthA = Math::Min(lenA, (int)strA.Length() - indexA);
    int lengthB = Math::Min(lenB, (int)strB.Length() - indexB);

    if(lengthA == lengthB && indexA == indexB && (&strA == &strB))
      return 0;

    cstring aptr = strA;
    cstring bptr = strB;
    string ap = (string)aptr + indexA;
    string end = ap + Math::Min(lengthA, lengthB);
    string bp = (string)bptr + indexB;
    while (ap < end) 
      {
      if (*ap != *bp)
        return *ap - *bp;
      ap++;
      bp++;
      }
    return lengthA - lengthB;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// CompareOrdinalUnchecked function
  int String::CompareOrdinalCaseInsensitiveUnchecked(String& strA, int indexA, int lenA, String& strB, int indexB, int lenB)
    {
    int lengthA = Math::Min(lenA, (int)strA.Length() - indexA);
    int lengthB = Math::Min(lenB, (int)strB.Length() - indexB);

    if(lengthA == lengthB && indexA == indexB && (&strA == &strB))
      return 0;

    cstring aptr = strA;
    cstring bptr = strB;
    string ap = (string)aptr + indexA;
    string end = ap + Math::Min (lengthA, lengthB);
    string bp = (string)bptr + indexB;
    while(ap < end)
      {
      if(*ap != *bp)
        {
        wchar_t c1 = Char::ToUpperInvariant(*ap);
        wchar_t c2 = Char::ToUpperInvariant(*bp);
        if (c1 != c2)
          return c1 - c2;
        }
      ap++;
      bp++;
      }
    return lengthA - lengthB;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// IndexOfOrdinalUnchecked function
  int String::IndexOfOrdinalUnchecked(const String& value)
    {
    return IndexOfOrdinalUnchecked(value, 0, (int)_length);
    }
  int String::IndexOfOrdinalUnchecked(const String& value, int startIndex, int count)
    {
    int valueLen = value.Length();
    if(count < valueLen)
      return -1;

    if(valueLen <= 1) 
      {
      if (valueLen == 1)
        return IndexOfUnchecked(value[0], startIndex, count);
      return startIndex;
      }

    string thisptr = _start_char;
    cstring valueptr = value;
    string ap = thisptr + startIndex;
    string thisEnd = ap + count - valueLen + 1;
    while(ap != thisEnd)
      {
      if(*ap == *valueptr) 
        {
        for(int i = 1; i < valueLen; i++) 
          {
          if(ap[i] != valueptr[i])
            {
            ap++;
            break;
            }
          }
        return (int)(ap - thisptr);
        }
      ap++;
      }
    return -1;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// IndexOfUnchecked function
  int String::IndexOfUnchecked(wchar_t value, int startIndex, int count)
    {
    int val = (int)value;
    wchar_t* start = _start_char;
    wchar_t* ptr = start + startIndex;
    wchar_t* end_ptr = ptr + (count >> 3 << 3);
    while(ptr != end_ptr)
      {
      if(*ptr == val)
        return (int)(ptr - start);
      if(ptr[1] == val)
        return (int)(ptr - start + 1);
      if(ptr[2] == val)
        return (int)(ptr - start + 2);
      if(ptr[3] == val)
        return (int)(ptr - start + 3);
      if(ptr[4] == val)
        return (int)(ptr - start + 4);
      if(ptr[5] == val)
        return (int)(ptr - start + 5);
      if(ptr[6] == val)
        return (int)(ptr - start + 6);
      if(ptr[7] == val)
        return (int)(ptr - start + 7);
      ptr += 8;
      }

    end_ptr += count & 0x0007;
    while(ptr != end_ptr)
      {
      if(*ptr == val)
        return (int)(ptr - start);

      ptr++;
      }
    return -1;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// CreateString function
  String String::CreateString(sbyte* value, uint32 startIndex, uint32 length, Text::Encoding* enc)
    {
    if(enc == nullptr) 
      {
      if(value == nullptr)
        throw ArgumentNullException(L"value");
      if(length == 0)
        return Empty();

      enc = Text::Encoding::Default();
      assert(enc != nullptr);
      }

    ByteArray bytes(length);

    if(length != 0)
      {
      byte* bytePtr = bytes.ToPtr();
      //try 
      //{
      String::memcpy(bytePtr, (byte*) (value + startIndex), length);
      //} 
      //catch(NullReferenceException)
      //{
      //throw ArgumentOutOfRangeException(L"ptr", L"Value, startIndex and length do not refer to a valid string.");
      //}
      }

    // GetString () is called even when length == 0
    return enc->GetString(bytes);
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// JoinUnchecked function
  String String::JoinUnchecked(String separator, StringArray& value, int startIndex, int count)
    {
    // Unchecked parameters
    // startIndex, count must be >= 0; startIndex + count must be <= value.length
    // separator and value must not be null

    int length = 0;
    int maxIndex = startIndex + count;
    // Precount the number of characters that the resulting string will have
    for(int i = startIndex; i < maxIndex; i++) 
      {
      String s = value[i];
      length += s.Length();
      }
    length += separator.Length() * (count - 1);
    if (length <= 0)
      return Empty();

    String tmp = InternalAllocateStr(length + 1);

    maxIndex--;
    cstring dest = tmp;
    cstring sepsrc = separator;
    // Copy each string from value except the last one and add a separator for each
    int pos = 0;
    for (int i = startIndex; i < maxIndex; i++)
      {
      String source = value[i];
      if (source.Length() > 0) 
        {
        cstring src = source;
        CharCopy((string)dest + pos, src, source.Length());
        pos += source.Length();
        }
      if(separator.Length() > 0)
        {
        CharCopy((string)dest + pos, sepsrc, separator.Length());
        pos += separator.Length();
        }
      }
    // Append last string that does not get an additional separator
    String sourceLast = value[maxIndex];

    if(sourceLast.Length() > 0)
      {
      cstring src = sourceLast;
      CharCopy((string)dest + pos, src, sourceLast.Length());
      }
    return tmp;
    }
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  /// CreateString function
  StringArray String::SplitByCharacters(CharArray& sep, int count, bool removeEmpty)
    {
    IntArray split_points;
    int total_points = 0;
    --count;

    if(sep.IsNull() || sep.Length() == 0) 
      {
      wchar_t* src = _start_char;
      wchar_t* src_ptr = src;
      int len = _length;

      while(len > 0) 
        {
        if(Char::IsWhiteSpace (*src_ptr++)) 
          {
          if(split_points.IsNull())
            {
            split_points.Length(8);
            } 
          else if((int)split_points.Length() == total_points)
            {
            split_points.Length(split_points.Length() * 2);
            }

          split_points[total_points++] = _length - len;
          if(total_points == count && !removeEmpty)
            break;
          }
        --len;
        }
      } 
    else 
      {
      wchar_t* src = _start_char; 
      wchar_t* sep_src = sep.ToPtr();
      wchar_t* src_ptr = src;
      wchar_t* sep_ptr_end = sep_src + sep.Length();
      int len = _length;
      while(len > 0) 
        {
        wchar_t* sep_ptr = sep_src;
        do 
          {
          if(*sep_ptr++ == *src_ptr)
            {
            if(split_points.IsNull()) 
              {
              split_points.Length(8);
              } 
            else if((int)split_points.Length() == total_points) 
              {
              split_points.Length(split_points.Length() * 2);
              }

            split_points[total_points++] = _length - len;
            if(total_points == count && !removeEmpty)
              len = 0;

            break;
            }
          }while (sep_ptr != sep_ptr_end);

          ++src_ptr;
          --len;
        }
      }

    if(total_points == 0)
      {
      StringArray ret(1);
      ret[0] = *this;
      return ret;
      }

    StringArray res(Math::Min(total_points, count) + 1);
    int prev_index = 0;
    int i = 0;
    if (!removeEmpty) 
      {
      for (; i < total_points; ++i) 
        {
        auto start = split_points[i];
        res[i] = SubstringUnchecked(prev_index, start - prev_index);
        prev_index = start + 1;
        }

      res[i] = SubstringUnchecked(prev_index, _length - prev_index);
      } 
    else 
      {
      int used = 0;
      int length;
      for(; i < total_points; ++i) 
        {
        auto start = split_points[i];
        length = start - prev_index;
        if(length != 0)
          {
          if(used == count)
            break;

          res[used++] = SubstringUnchecked(prev_index, length);
          }

        prev_index = start + 1;
        }

      length = _length - prev_index;
      if(length != 0)
        res[used++] = SubstringUnchecked(prev_index, length);

      if(used != (int)res.Length())
        res.Length(used);
      }

    return res;
    }
  // ------------------------------------------------------------------------

  String String::ToLowerInvariant()
    {
    if(_length == 0)
      return Empty();

    String tmp;
    tmp.InternalSetLength(_length + 1);
    wchar_t* source = _start_char;
    const wchar_t* dest = (cstring)tmp;
      {
      wchar_t* destPtr = (string)dest;
      wchar_t* sourcePtr = (wchar_t*)source;

      for(int n = 0; n < (int)_length; n++)
        {
        *destPtr = Char::ToLowerInvariant(*sourcePtr);
        sourcePtr++;
        destPtr++;
        }
      *destPtr = L'\0';
      }
      return tmp;
    }








  //// Compare string
  //int String::compare(const String& s)
  //  {
  //  return compare(_start_char, s._start_char);
  //  }
  //// Copy string
  //void String::copy(const String& s)
  //  {
  //  _length = s._length;
  //  free();
  //  _buffer = new wchar_t[_length + 1];
  //  copy(_start_char, s._start_char);
  //  check_null();
  //  }

  String String::ToString()
    {
    return *this;
    }







  System::String String::Format(String& format, ObjectArray& args)
    {
    Text::GCStringBuilder sb(FormatHelper(nullptr, format, args));
    return sb->ToString();
    }

  System::Text::StringBuilder* String::FormatHelper(System::Text::StringBuilder* result, String& format, ObjectArray& args)
    {
    using namespace System::Text;

    /*if(format.IsNull())
    throw ArgumentNullException(L"format");*/
    if(args.IsNull())
      throw ArgumentNullException(L"arguments");

    if(result == nullptr)
      {
      int i;
      int len = 0;
      for(i = 0; i < (int)args.Length(); ++i)
        {
        String s;
        if(Object::IsInstance(args[i], s))
          s = args[i].ToString();
        if((cstring)s != nullptr)
          len += s.Length();
        else
          break;
        }
      if(i == (int)args.Length())
        result = new StringBuilder(len + format.Length());
      else
        result = new StringBuilder();
      }

    int ptr = 0;
    uint32 start = ptr;
    while(ptr < format.Length())
      {
      wchar_t c = format[ptr++];

      if(c == L'{')
        {
        result->Append(format, start, ptr - start - 1);

        // check for escaped open bracket
        if(format[ptr] == L'{')
          {
          start = ptr++;
          continue;
          }

        int n, width;
        bool left_align;
        String arg_format;

        // parse specifier
        ParseFormatSpecifier(format, (int&)ptr, n, width, left_align, arg_format);
        if(n >= (int)args.Length())
          throw SystemException(L"Index (zero based) must be greater than or equal to zero and less than the size of the argument list.");
        // TODO: throw FormatException ("Index (zero based) must be greater than or equal to zero and less than the size of the argument list.");

        // format argument
        String temp = args[n].ToString();
        result->Append(temp);

        start = ptr;
        }
      else if (c == '}' && ptr < format.Length() && format[ptr] == '}')
        {
        result->Append (format, start, ptr - start - 1);
        start = ptr ++;
        }
      else if (c == '}')
        {
        throw SystemException(L"Input string was not in a correct format.");
        // TODO: throw FormatException ("Input string was not in a correct format.");
        }
      }

    if((int)start < format.Length())
      result->Append(format, start, format.Length() - start);

    return result;
    }

  bool String::IsNullOrWhiteSpace(String& value)
    {
    if(/*value.IsNull() || */(value.Length() == 0))
      return true;
    uint32 len = value.Length();
    for(uint32 i = 0; i < len; ++i)
      if(!System::Char::IsWhiteSpace(value[i]))
        return false;
    return true;
    }





  int String::LastIndexOf(wchar_t c)
    {
    if(_length == 0)
      return -1;

    return IndexOfUnchecked(c, 0, _length);
    }

  int String::LastIndexOfAny(System::CharArray anyOf)
    {
    if(anyOf.IsNull())
      throw System::ArgumentNullException ();

    if(_length == 0)
      return -1;

    return LastIndexOfAnyUnchecked(anyOf, _length - 1, _length);
    }

  int String::IndexOfAny(System::CharArray anyOf)
    {
    if(anyOf.IsNull())
      throw System::ArgumentNullException();

    if(_length == 0)
      return -1;

    return IndexOfAnyUnchecked(anyOf, 0, _length);
    }

  String String::TrimStart(CharArray& trimChars)
    {
    if(_length == 0) 
      return Empty();
    int start;
    if(trimChars.IsNull() || trimChars.Length() == 0)
      start = FindNotWhiteSpace(0, _length, 1);
    else
      start = FindNotInTable(0, _length, 1, trimChars);

    if (start == 0)
      return *this;

    return SubstringUnchecked(start, _length - start);
    }





  // Replace characters in current string
  void String::replace(wchar_t o, wchar_t n)
    {
    for(uint32 i = 0; i < _length; ++i)
      {
      if(_buffer[i] == o)
        _buffer[i] = n;
      }
    }

  String String::SubstringUnchecked(int startIndex, int length)
    {
    if (length == 0)
      return Empty();

    CharArray tmp(length + 1); 
    //string tmp = InternalAllocateStr(length);
    wchar_t* dest = tmp.ToPtr();
    wchar_t* src = _start_char;
    CharCopy (dest, src + startIndex, length);
    tmp[length] = L'\0';
    return String(tmp.ToConstPtr());
    }

  // Get hash code of string
  uint32 String::GetHashCode()
    {
    uint32 retval = 0;
    for(uint32 i = 0; _buffer[i] != L'\0'; ++i)
      retval = (retval & mask) ^ (retval << shift) ^ _buffer[i];
    return retval;
    }
  // To a byte array
  ByteArray String::to_byte_array()
    {
    ByteArray retval(_length + 1);
    // ASCII 7bit
    ::WideCharToMultiByte(20127, 0, _buffer, -1, (LPSTR)retval.ToPtr(), (int)retval.Length(), NULL, NULL);
    return retval; 
    }
  // To wchar_t array
  CharArray String::to_wchar_array()
    {
    CharArray retval(_length);
    for(uint32 i = 0; i < _length; ++i)
      retval[i] = _buffer[i];
    return retval;
    }

  int String::FindNotWhiteSpace(uint32 pos, int target, int change)
    {
    wchar_t* src = _start_char;
      {
      while((int)pos != target)
        {
        if(!Char::IsWhiteSpace(src[pos]))
          return (int)pos;

        pos += change;
        }
      }
      return (int)pos;
    }

  int String::FindNotInTable(uint32 pos, int target, int change, CharArray& table)
    {
    wchar_t* tablePtr = table.ToPtr();
    wchar_t* thisPtr = _start_char;
    while ((int)pos != target)
      {
      wchar_t c = thisPtr[pos];
      uint32 x = 0;
      while (x < table.Length())
        {
        if (c == tablePtr[x])
          break;
        x++;
        }
      if (x == table.Length())
        return pos;
      pos += change;
      }
    return pos;
    }



  int String::IndexOfAnyUnchecked(System::CharArray& anyOf, int startIndex, int count)
    {
    if(anyOf.Length() == 0)
      return -1;

    if(anyOf.Length() == 1)
      return IndexOfUnchecked (anyOf[0], startIndex, count);

    wchar_t* any = anyOf.ToPtr();;
    int highest = *any;
    int lowest = *any;

    wchar_t* end_any_ptr = any + anyOf.Length();
    wchar_t* any_ptr = any;
    while(++any_ptr != end_any_ptr)
      {
      if(*any_ptr > highest)
        {
        highest = *any_ptr;
        continue;
        }

      if(*any_ptr < lowest)
        lowest = *any_ptr;
      }

    wchar_t* start = _start_char;
    wchar_t* ptr = start + startIndex;
    wchar_t* end_ptr = ptr + count;

    while(ptr != end_ptr)
      {
      if(*ptr > highest || *ptr < lowest)
        {
        ptr++;
        continue;
        }

      if(*ptr == *any)
        return (int)(ptr - start);

      any_ptr = any;
      while(++any_ptr != end_any_ptr)
        {
        if(*ptr == *any_ptr)
          return (int)(ptr - start);
        }
      ptr++;
      }
    return -1;
    }

  int String::LastIndexOfUnchecked(wchar_t value, int startIndex, int count)
    {
    int value_32 = (int)value;

    wchar_t* start = _start_char;
    wchar_t* ptr = start + startIndex;
    wchar_t* end_ptr = ptr - (count >> 3 << 3);

    while(ptr != end_ptr)
      {
      if (*ptr == value_32)
        return (int)(ptr - start);
      if (ptr[-1] == value_32)
        return (int)(ptr - start) - 1;
      if (ptr[-2] == value_32)
        return (int)(ptr - start) - 2;
      if (ptr[-3] == value_32)
        return (int)(ptr - start) - 3;
      if (ptr[-4] == value_32)
        return (int)(ptr - start) - 4;
      if (ptr[-5] == value_32)
        return (int)(ptr - start) - 5;
      if (ptr[-6] == value_32)
        return (int)(ptr - start) - 6;
      if (ptr[-7] == value_32)
        return (int)(ptr - start) - 7;

      ptr -= 8;
      }

    end_ptr -= count & 0x07;
    while(ptr != end_ptr)
      {
      if(*ptr == value_32)
        return (int)(ptr - start);

      ptr--;
      }
    return -1;
    }

  int String::LastIndexOfAnyUnchecked(System::CharArray& anyOf, int startIndex, int count)
    {
    if(anyOf.Length() == 1)
      return LastIndexOfUnchecked(anyOf[0], startIndex, count);
    wchar_t* start = _start_char;
    wchar_t* testStart = anyOf.ToPtr();
    wchar_t* ptr = start + startIndex;
    wchar_t* ptrEnd = ptr - count;
    wchar_t* test;
    wchar_t* testEnd = testStart + anyOf.Length();

    while (ptr != ptrEnd)
      {
      test = testStart;
      while (test != testEnd)
        {
        if (*test == *ptr)
          return (int)(ptr - start);
        test++;
        }
      ptr--;
      }
    return -1;
    }
#pragma warning (disable:4189)
  void String::ParseFormatSpecifier(String& str, int& ptr, int& n, int& width, bool& left_align, String& format)
    {
    int max = str.Length();

    // parses format specifier of form:
    //   N,[\ +[-]M][:F]}
    //
    // where:
    // N = argument number (non-negative integer)

    n = ParseDecimal(str, ptr);
    if(n < 0)
      throw SystemException(L"Input string was not in a correct format.");
    // TODO: throw FormatException ("Input string was not in a correct format.");

    // M = width (non-negative integer)

    if(ptr < max && str[ptr] == L',')
      {
      // White space between ',' and number or sign.
      ++ptr;
      while(ptr < max && Char::IsWhiteSpace(str [ptr]))
        ++ptr;
      int start = ptr;

      format = str.Substring (start, ptr - start);

      left_align = (ptr < max && str [ptr] == '-');
      if (left_align)
        ++ ptr;

      width = ParseDecimal (str, ptr);
      if (width < 0)
        throw SystemException(L"Input string was not in a correct format.");
      // TODO: throw FormatException ("Input string was not in a correct format.");
      }
    else
      {
      width = 0;
      left_align = false;
      format = Empty();
      }

    // F = argument format (string)
    if (ptr < max && str[ptr] == ':')
      {
      int start = ++ ptr;
      while (ptr < max && str[ptr] != '}')
        ++ ptr;

      // TODO: format += str.Substring (start, ptr - start);
      }
    //else
    // TODO: format = null;

    if ((ptr >= max) || str[ptr ++] != '}')
      throw SystemException(L"Input string was not in a correct format.");
    // TODO: throw FormatException ("Input string was not in a correct format.");
    }
#pragma warning (default:4189)

  int String::ParseDecimal(String& str, int& ptr)
    {
    int p = ptr;
    int n = 0;
    int max = str.Length();

    while (p < max)
      {
      wchar_t c = str[p];
      if (c < L'0' || L'9' < c)
        break;

      n = n * 10 + c - L'0';
      ++ p;
      }

    if (p == ptr || p == max)
      return -1;

    ptr = p;
    return n;
    }

  String String::InternalAllocateStr(uint32 length)
    {
    if(length != 0)
      {
      String retval;
      retval._length = length - 1;
      retval._buffer = new wchar_t[length];
      retval._buffer[length - 1] = L'\0';
      retval._start_char = retval._buffer;
      return retval;
      }
    return String::Empty();
    }

  void String::InternalSetChar(int idx, wchar_t val)
    {
    if(idx >= Length())
      throw ArgumentOutOfRangeException(L"idx");

    wchar_t* pStr = _start_char;
    wchar_t p = pStr[idx];
    pStr[idx++] = val;
    pStr[idx] = p;
    }

  void String::CharCopy(String& target, int targetIndex, CharArray& source, int sourceIndex, int count)
    {
    cstring dest = (cstring)target;
    string src = source.ToPtr();
    CharCopy((string)dest + targetIndex, src + sourceIndex, count);
    }

  void String::CharCopy(String& target, int targetIndex, String& source, int sourceIndex, int count)
    {
    cstring dest = target;
    cstring src = (cstring)source;
    CharCopy((string)dest + targetIndex, src + sourceIndex, count);
    }

  void String::CharCopy(wchar_t *dest, const wchar_t *src, int count)
    {
    // Same rules as for memcpy, but with the premise that 
    // chars can only be aligned to even addresses if their
    // enclosing types are correctly aligned
    if ((((intptr)(byte*)dest | (intptr)(byte*)src) & 3) != 0)
      {
      if (((intptr)(byte*)dest & 2) != 0 && ((intptr)(byte*)src & 2) != 0 && count > 0)
        {
        ((short*)dest) [0] = ((short*)src) [0];
        dest++;
        src++;
        count--;
        }
      if ((((intptr)(byte*)dest | (intptr)(byte*)src) & 2) != 0)
        {
        memcpy2 ((byte*)dest, (byte*)src, count * 2);
        return;
        }
      }
    memcpy4 ((byte*)dest, (byte*)src, count * 2);
    }

  void String::CharCopyReverse(String& target, int targetIndex, String& source, int sourceIndex, int count)
    {
    cstring dest = (cstring)target;
    cstring src = (cstring)source;
    CharCopyReverse((string)dest + targetIndex, (string)src + sourceIndex, count);
    }

  void String::CharCopyReverse(wchar_t *dest, wchar_t *src, int count)
    {
    dest += count;
    src += count;
    for(int i = count; i > 0; i--)
      {
      dest--;
      src--;
      *dest = *src;
      }	
    }

  void String::InternalSetLength(uint32 newLength)
    {
    wchar_t* new_ptr = new wchar_t[newLength + 1];
    uint32 min = _length < newLength ? _length : newLength;
    for(uint32 i = 0; i < min; ++i)
      new_ptr[i] = _buffer[i];
    free();
    _buffer = new_ptr;
    _start_char = _buffer;
    _length = newLength;
    _buffer[_length] = L'\0';
    }

  void String::memcpy4 (byte *dest, byte *src, int size)
    {
    /*while (size >= 32) {
    // using long is better than int and slower than double
    // FIXME: enable this only on correct alignment or on platforms
    // that can tolerate unaligned reads/writes of doubles
    ((double*)dest) [0] = ((double*)src) [0];
    ((double*)dest) [1] = ((double*)src) [1];
    ((double*)dest) [2] = ((double*)src) [2];
    ((double*)dest) [3] = ((double*)src) [3];
    dest += 32;
    src += 32;
    size -= 32;
    }*/
    while (size >= 16)
      {
      ((int*)dest) [0] = ((int*)src) [0];
      ((int*)dest) [1] = ((int*)src) [1];
      ((int*)dest) [2] = ((int*)src) [2];
      ((int*)dest) [3] = ((int*)src) [3];
      dest += 16;
      src += 16;
      size -= 16;
      }
    while (size >= 4)
      {
      ((int*)dest) [0] = ((int*)src) [0];
      dest += 4;
      src += 4;
      size -= 4;
      }
    while (size > 0)
      {
      ((byte*)dest) [0] = ((byte*)src) [0];
      dest += 1;
      src += 1;
      --size;
      }
    }

  void String::memcpy2 (byte *dest, byte *src, int size)
    {
    while (size >= 8)
      {
      ((short*)dest) [0] = ((short*)src) [0];
      ((short*)dest) [1] = ((short*)src) [1];
      ((short*)dest) [2] = ((short*)src) [2];
      ((short*)dest) [3] = ((short*)src) [3];
      dest += 8;
      src += 8;
      size -= 8;
      }
    while (size >= 2)
      {
      ((short*)dest) [0] = ((short*)src) [0];
      dest += 2;
      src += 2;
      size -= 2;
      }
    if (size > 0)
      ((byte*)dest) [0] = ((byte*)src) [0];
    }

  // Static copy characters function
  wchar_t* String::copy_nchar(wchar_t* s1, const wchar_t* s2, uint32 n)
    {
    wchar_t* s = s1;
    while(n > 0 && *s2 != L'\0')
      {
      *s++ = *s2++;
      --n;
      }
    while(n > 0)
      {
      *s++ = L'\0';
      --n;
      }
    return s1;
    }
  // Static length function
  sizet String::WcsLen(cstring s)
    {
    const wchar_t* p = s;
    while(*p)
      p++;

    return (sizet)(p - s);
    }
  sizet String::StrLen(sbyte* s)
    {
    const char* p = s;
    while(*p)
      p++;

    return (sizet)(p - s);
    }
  // Check for null terminator
  void String::check_null()
    {
    if(_buffer[_length] != L'\0')
      _buffer[_length] = L'\0';
    }
  // Delete buffer
  void String::free()
    {
    delete [] _buffer;
    _buffer = nullptr;
    }

  void String::memcpy(byte *dest, byte *src, int size) 
    {
    // FIXME: if pointers are not aligned, try to align them
    // so a faster routine can be used. Handle the case where
    // the pointers can't be reduced to have the same alignment
    // (just ignore the issue on x86?)
    if ((((intptr)dest | (intptr)src) & 3) != 0)
      {
      if (((intptr)dest & 1) != 0 && ((intptr)src & 1) != 0 && size >= 1) 
        {
        dest [0] = src [0];
        ++dest;
        ++src;
        --size;
        }
      if (((intptr)dest & 2) != 0 && ((intptr)src & 2) != 0 && size >= 2)
        {
        ((short*)dest) [0] = ((short*)src) [0];
        dest += 2;
        src += 2;
        size -= 2;
        }
      if ((((intptr)dest | (intptr)src) & 1) != 0) 
        {
        memcpy1 (dest, src, size);
        return;
        }
      if ((((intptr)dest | (intptr)src) & 2) != 0) 
        {
        memcpy2 (dest, src, size);
        return;
        }
      }
    memcpy4 (dest, src, size);
    }

  void String::memcpy1(byte *dest, byte *src, int size)
    {
    while (size >= 8)
      {
      ((byte*)dest) [0] = ((byte*)src) [0];
      ((byte*)dest) [1] = ((byte*)src) [1];
      ((byte*)dest) [2] = ((byte*)src) [2];
      ((byte*)dest) [3] = ((byte*)src) [3];
      ((byte*)dest) [4] = ((byte*)src) [4];
      ((byte*)dest) [5] = ((byte*)src) [5];
      ((byte*)dest) [6] = ((byte*)src) [6];
      ((byte*)dest) [7] = ((byte*)src) [7];
      dest += 8;
      src += 8;
      size -= 8;
      }
    while (size >= 2) 
      {
      ((byte*)dest) [0] = ((byte*)src) [0];
      ((byte*)dest) [1] = ((byte*)src) [1];
      dest += 2;
      src += 2;
      size -= 2;
      }
    if (size > 0)
      ((byte*)dest) [0] = ((byte*)src) [0];
    }

  // Operator append
  String operator+(const String& s1, const String& s2)
    {
    Text::StringBuilder sb(s1);
    sb.Append((cstring)s2);
    //CharArray a1(s1._length + s2._length + 1);
    //String::copy(a1, s1);
    //String::append(a1, s2);
    return sb.ToString();
    }

  bool operator==(const String& s1, const String& s2)
    {
    return String::Equals(s1, s2);
    }

  bool operator!=(const String& s1, const String& s2)
    {
    return !String::Equals(s1, s2);
    }  
  }
