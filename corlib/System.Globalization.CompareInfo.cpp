#include "pch.h"
#include "System.Globalization.CompareInfo.h"
#include "System.Globalization.CultureInfo.h"
#include "System.Exception.h"
#include "UnicodeData.h"
#include "System.Char.h"

typedef enum {
  UNICODE_CONTROL,
  UNICODE_FORMAT,
  UNICODE_UNASSIGNED,
  UNICODE_PRIVATE_USE,
  UNICODE_SURROGATE,
  UNICODE_LOWERCASE_LETTER,
  UNICODE_MODIFIER_LETTER,
  UNICODE_OTHER_LETTER,
  UNICODE_TITLECASE_LETTER,
  UNICODE_UPPERCASE_LETTER,
  UNICODE_COMBINING_MARK,
  UNICODE_ENCLOSING_MARK,
  UNICODE_NON_SPACING_MARK,
  UNICODE_DECIMAL_NUMBER,
  UNICODE_LETTER_NUMBER,
  UNICODE_OTHER_NUMBER,
  UNICODE_CONNECT_PUNCTUATION,
  UNICODE_DASH_PUNCTUATION,
  UNICODE_CLOSE_PUNCTUATION,
  UNICODE_FINAL_PUNCTUATION,
  UNICODE_INITIAL_PUNCTUATION,
  UNICODE_OTHER_PUNCTUATION,
  UNICODE_OPEN_PUNCTUATION,
  UNICODE_CURRENCY_SYMBOL,
  UNICODE_MODIFIER_SYMBOL,
  UNICODE_MATH_SYMBOL,
  UNICODE_OTHER_SYMBOL,
  UNICODE_LINE_SEPARATOR,
  UNICODE_PARAGRAPH_SEPARATOR,
  UNICODE_SPACE_SEPARATOR
  } UnicodeType;

UnicodeType unichar_type(wchar_t c)
  {
  int i;

  int16 cp = (int16) c;
  for (i = 0; i < unicode_category_ranges_count; i++) {
    if ((uint32)cp < unicode_category_ranges [i].start)
      continue;
    if (unicode_category_ranges [i].end <= (uint32)cp)
      continue;
    return (UnicodeType)unicode_category [i] [cp - unicode_category_ranges [i].start];
    }

  /*
  // 3400-4DB5: OtherLetter
  // 4E00-9FC3: OtherLetter
  // AC00-D7A3: OtherLetter
  // D800-DFFF: OtherSurrogate
  // E000-F8FF: OtherPrivateUse
  // 20000-2A6D6 OtherLetter
  // F0000-FFFFD OtherPrivateUse
  // 100000-10FFFD OtherPrivateUse
  */
  if (0x3400 <= cp && cp < 0x4DB5)
    return UNICODE_OTHER_LETTER;
  if (0x4E00 <= cp && cp < 0x9FC3)
    return UNICODE_OTHER_LETTER;
  if (0xAC00<= cp && cp < 0xD7A3)
    return UNICODE_OTHER_LETTER;
  if (0xD800 <= cp && cp < 0xDFFF)
    return UNICODE_SURROGATE;
  if (0xE000 <= cp && cp < 0xF8FF)
    return UNICODE_PRIVATE_USE;
  /* since the argument is UTF-16, we cannot check beyond FFFF */

  /* It should match any of above */
  return (UnicodeType)0;
  }

namespace System
  {
  namespace Globalization
    {
    CompareInfo::CompareInfo(CultureInfo& ci)
      :_culture(ci.LCID())
      ,_name(ci.Name())
      {
      // Not sure if this code is needed yet
      //if (UseManagedCollation) 
      //  {
      //  lock (monitor)
      //    {
      //    if(collators == null)
      //      collators = new Dictionary<int, SimpleCollator> ();

      //    if(!collators.TryGetValue(ci.LCID, out collator))
      //      {
      //      collator = new SimpleCollator (ci);
      //      collators [ci.LCID] = collator;
      //      }
      //    }
      //  } 
      //else 
      //  {
      //  /*
      //  this.icu_name = ci.IcuName;
      //  this.construct_compareinfo (icu_name);
      //  */
      //  }
      }
    CompareInfo::~CompareInfo()
      {
      }
    int CompareInfo::Compare(String& string1, String& string2)
      {
      return Compare(string1, string2, CompareOptions::None);
      }
    int CompareInfo::Compare(String& string1, String& string2, CompareOptions options)
      {
      if ( ((ulong)options & ValidCompareOptions) != (ulong)options )
        throw ArgumentException(L"options");

      /* Short cut... */
      if(string1.Length() == 0 && string2.Length() == 0)
        return 0;

      return(internal_compare_switch (string1, 0, string1.Length(), string2, 0, string2.Length(), options));
      }

   int CompareInfo::Compare(String& string1, int offset1, int length1, String& string2, int offset2, int length2, CompareOptions options)
		{
			if( ((ulong)options & ValidCompareOptions) != (ulong)options )
				throw ArgumentException(L"options");

			/* Not in the spec, but ms does these short
			 * cuts before checking the offsets (breaking
			 * the offset >= string length specified check
			 * in the process...)
			 */
			if((string1.Length() == 0 || offset1 == (int)string1.Length() || length1 == 0) &&
				 (string2.Length() == 0 || offset2 == (int)string2.Length() || length2 == 0))
					return(0);

			if(offset1 < 0 || length1 < 0 ||
			   offset2 < 0 || length2 < 0) {
				throw ArgumentOutOfRangeException(L"Offsets and lengths must not be less than zero");
			}
			
			if(offset1 > (int)string1.Length()) {
				throw ArgumentOutOfRangeException(L"Offset1 is greater than or equal to the length of string1");
			}
			
			if(offset2 > (int)string2.Length()) {
				throw ArgumentOutOfRangeException(L"Offset2 is greater than or equal to the length of string2");
			}
			
			if(length1 > ((int)string1.Length() - offset1)) {
				throw ArgumentOutOfRangeException(L"Length1 is greater than the number of characters from offset1 to the end of string1");
			}
			
			if(length2 > ((int)string2.Length() - offset2)) {
				throw ArgumentOutOfRangeException(L"Length2 is greater than the number of characters from offset2 to the end of string2");
			}
			
			return(internal_compare_switch (string1, offset1, length1, string2, offset2, length2, options));
		}


    bool CompareInfo::IsSuffix(String& source, String& suffix)
      {
      return IsSuffix(source, suffix, CompareOptions::None);
      }

    bool CompareInfo::IsSuffix(String& source, String& suffix, CompareOptions options)
      {
      if(((ulong)options & ValidCompareOptions_NoStringSort) != (ulong)options)
        throw ArgumentException(L"options");

      //if (UseManagedCollation)
      //return collator.IsSuffix (source, suffix, options);

      if(source.Length() < suffix.Length()) 
        {
        return false;
        } 
      else 
        {
        return(Compare(source, source.Length() - suffix.Length(), suffix.Length(), suffix, 0, suffix.Length(), options) == 0);
        }
      }


    int CompareInfo::internal_compare_switch(String& str1, int offset1, int length1, String& str2, int offset2, int length2, CompareOptions options)
      {
      if (options == CompareOptions::Ordinal)
        return String::CompareOrdinalUnchecked(str1, offset1, length1, str2, offset2, length2);
      if (options == CompareOptions::OrdinalIgnoreCase)
        return String::CompareOrdinalCaseInsensitiveUnchecked(str1, offset1, length1, str2, offset2, length2);

      return internal_compare(str1, offset1, length1, str2, offset2, length2, options);
      }
    int CompareInfo::internal_compare(String& str1, int offset1, int length1, String& str2, int offset2, int length2, CompareOptions options)
      {
      return invariant_compare(str1, offset1, length1, str2, offset2, length2, options);
      }

    int CompareInfo::invariant_compare_char(wchar_t c1, wchar_t c2, CompareOptions options)
      {
      int result;

      /* Ordinal can not be mixed with other options, and must return the difference, not only -1, 0, 1 */
      if((ulong)options & (ulong)CompareOptions::Ordinal) 
        return(int) c1 - c2;

      if((ulong)options & (ulong)CompareOptions::IgnoreCase)
        {
        UnicodeType c1type, c2type;

        c1type = unichar_type (c1);
        c2type = unichar_type (c2);

        result = (int) (c1type != UNICODE_LOWERCASE_LETTER ? Char::ToLower(c1) : c1) -
          (c2type != UNICODE_LOWERCASE_LETTER ? Char::ToLower(c2) : c2);
        } 
      else 
        {
        /*
        * No options. Kana, symbol and spacing options don't
        * apply to the invariant culture.
        */

        /*
        * FIXME: here we must use the information from c1type and c2type
        * to find out the proper collation, even on the InvariantCulture, the
        * sorting is not done by computing the unicode values, but their
        * actual sort order.
        */
        result = (int) c1 - c2;
        }

      return ((result < 0) ? -1 : (result > 0) ? 1 : 0);
      }

    int CompareInfo::invariant_compare(String& str1, int offset1, int length1, String& str2, int offset2, int length2, CompareOptions options)
      {
      int length = 0;
      int charcmp = 0;
      if(length1 >= length2) 
        {
        length=length1;
        } 
      else 
        {
        length=length2;
        }

      string ustr1 = (string)(cstring)str1 + offset1;
      string ustr2 = (string)(cstring)str2 + offset2;

      int pos = 0;

      for(pos = 0; pos != length; pos++) 
        {
        if(pos >= length1 || pos >= length2)
          break;

        charcmp = invariant_compare_char(ustr1[pos], ustr2[pos], options);
        if(charcmp != 0)
          {
          return charcmp;
          }
        }

      /* the lesser wins, so if we have looped until length we just
      * need to check the last char
      */
      if(pos == length)
        {
        return(invariant_compare_char(ustr1[pos - 1], ustr2[pos - 1], options));
        }

      /* Test if one of the strings has been compared to the end */
      if(pos >= length1) 
        {
        if (pos >= length2) 
          {
          return(0);
          } 
        else {
          return(-1);
          }
        } 
      else if (pos >= length2)
        {
        return(1);
        }
      return invariant_compare_char(ustr1[pos], ustr2[pos], options);
      }
    }
  }
