// Perch Lake Computer System

#pragma once
#include "AutoPtr.h"
#include "System.Array.h"
#include "System.IComparable.h"
#include "System.IEquatable.h"
#include "System.Globalization.CompareOptions.h"

namespace System
  {
  enum class StringSplitOptions
    {
    None = 0,
    RemoveEmptyEntries = 1
    };
  namespace Text
    {
    class Encoding;
    class UnicodeEncoding;
    class StringBuilder;
    }
  namespace IO
    {
    class Path;
    }
  namespace Globalization
    {
    class CultureInfo;
    class CompareInfo;
    }
  class String;
  typedef Array<String> StringArray;
  class CRAPOCOREDLL_API String : public Object, public IComparable<String>, public IEquatable<String>
    {
    private:
      uint32   _length;
      wchar_t* _buffer;
      wchar_t* _start_char;
    public:
      // Constructors
      String(const wchar_t* = nullptr);
      String(wchar_t);
      String(const wchar_t*, uint32, uint32);
      String(sbyte*);
      String(sbyte*, uint32, uint32);
      String(sbyte*, uint32, uint32, Text::Encoding*);
      String(const CharArray&);
      String(wchar_t, uint32);
      String(const String&);

      // Destructor & Operators
      ~String();
      String& operator = (const String&);
      String& operator += (const String&);
      String& operator += (const wchar_t&);
      operator cstring () const { return _start_char; }

      // Fields
      static String Empty();

      // Methods
      virtual int CompareTo(String&) override;
      static int Compare(String&, String&);
      static int CompareOrdinal(String& strA, int indexA, String& strB, int indexB, int length);
      static String Concat(String str0, String str1, String str2);
      bool Contains(const String&);
      // static String Copy(const String&) [Use Copy Constructor]
      bool EndsWith(String);
      virtual bool Equals(Object* obj) override;
      virtual bool Equals(String) override;
      static bool Equals(const String& a, const String& b);
      static String Format(String, Object*);
      static String Format(String, Object*, Object*);
      static String Format(String, Object*, Object*, Object*);
      static String Format(String&, ObjectArray&);
      virtual uint32 GetHashCode() override;
      int IndexOf(wchar_t);
      int IndexOf(wchar_t, int);
      int IndexOfAny(CharArray);
      static bool IsNullOrWhiteSpace(String& value);
      static String Join(String, StringArray&, int, int);
      int LastIndexOf(wchar_t);
      int LastIndexOf(wchar_t, int);
      int LastIndexOf(wchar_t, int, int);
      int LastIndexOfAny(CharArray anyOf);
      String Replace(wchar_t, wchar_t);
      StringArray Split(CharArray&);
      StringArray Split(CharArray&, StringSplitOptions);
      StringArray Split(CharArray&, int, StringSplitOptions);
      StringArray Split(StringArray&, StringSplitOptions);
      StringArray Split(StringArray&, int, StringSplitOptions);
      String Substring(uint32);
      CharArray ToCharArray();
      CharArray ToCharArray(uint32, uint32);
      virtual String ToString() override;
      String ToLowerInvariant();
      String Trim();
      String TrimStart(CharArray&);
      String TrimEnd();
      String TrimEnd(CharArray&);

      // Properties
      int Length();
      int Length() const;

      
     
      // Substring specified pos and has specified length
      String Substring(uint32, uint32);

      // Replace characters in current string
      void replace(wchar_t, wchar_t);

      // To a byte array
      ByteArray to_byte_array();
      // To wchar_t array
      CharArray to_wchar_array();
      
      // Static copy characters function
      static wchar_t* copy_nchar(wchar_t*, const wchar_t*, uint32);
      // Static length function
      static sizet WcsLen(cstring);
      static sizet StrLen(sbyte*);
    protected:
      int IndexOfOrdinalUnchecked(const String&);
      int IndexOfOrdinalUnchecked(const String&, int, int);
      int IndexOfUnchecked(wchar_t, int, int);
    private:
      static int Compare(String& strA, int indexA, String& strB, int indexB, int length, bool ignoreCase, Globalization::CultureInfo* culture);
      static int CompareOrdinalUnchecked(String& strA, int indexA, int lenA, String& strB, int indexB, int lenB);
      static int CompareOrdinalCaseInsensitiveUnchecked(String& strA, int indexA, int lenA, String& strB, int indexB, int lenB);
      String CreateString(sbyte* value, uint32 startIndex, uint32 length, Text::Encoding* enc);
      static String JoinUnchecked(String separator, StringArray& value, int startIndex, int count);
      StringArray SplitByCharacters(CharArray& sep, int count, bool removeEmpty);
    private:
      // Check for null terminator
      void check_null();
      // Delete buffer
      void free();

      static System::Text::StringBuilder* FormatHelper(System::Text::StringBuilder*, String&, ObjectArray&);


      int IndexOfAnyUnchecked(CharArray&, int, int);
      int LastIndexOfUnchecked(wchar_t, int, int);
      int LastIndexOfAnyUnchecked(CharArray&, int, int);
      int FindNotWhiteSpace(uint32 pos, int target, int change);
      int FindNotInTable(uint32 pos, int target, int change, CharArray& table);

      void InternalSetChar(int idx, wchar_t val);
      String SubstringUnchecked(int startIndex, int length);

      void InternalSetLength(uint32 newLength);

      static void ParseFormatSpecifier(String&, int&, int&, int&, bool&, String&);
      static int ParseDecimal(String&, int&);

      static String InternalAllocateStr(uint32);
      static void CharCopy(String&, int, String&, int, int);
      static void CharCopy(wchar_t*, const wchar_t*, int);
      static void CharCopy(String& target, int targetIndex, CharArray& source, int sourceIndex, int count);
      static void CharCopyReverse(wchar_t *dest, wchar_t *src, int count);
      static void CharCopyReverse(String& target, int targetIndex, String& source, int sourceIndex, int count);
      static void memcpy(byte *dest, byte *src, int size);
      static void memcpy1(byte *dest, byte *src, int size);
      static void memcpy4(byte*, byte*, int);
      static void memcpy2(byte*, byte*, int);

      // Friend operator append
      friend String operator+(const String&, const String&);
      friend System::Text::StringBuilder;
      friend System::Text::UnicodeEncoding;
      friend System::IO::Path;
      friend System::Globalization::CompareInfo;
    };

  // Operator append
  String operator+(const String& s1, const String& s2);
  bool operator==(const String& s1, const String& s2);
  CRAPOCOREDLL_API bool operator!=(const String& s1, const String& s2);

  typedef AutoPtr<String> GCString;
  }
