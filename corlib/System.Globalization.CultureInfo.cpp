// Perch Lake Computer System

#include "pch.h"
#include "System.Globalization.CultureInfo.h"
#include "System.Exception.h"
#include "System.Threading.Thread.h"
#include "System.Text.UTF8Encoding.h"

namespace System
  {
  namespace Globalization
    {
    CultureInfo CultureInfo::_invariantCultureInfo(127, false, false);

    CultureInfo::CultureInfo(uint32 culture, bool useUserOverride)
      {
      Ctor(culture, useUserOverride, false);
      }

    CultureInfo::CultureInfo(String name, bool useUserOverride)
      {
      Ctor(name, useUserOverride, false);
      }

    CultureInfo::CultureInfo(uint32 culture, bool useUserOverride, bool readOnly)
      {
      Ctor(culture, useUserOverride, readOnly);
      }

    CultureInfo::CultureInfo(String name, bool useUserOverride, bool readOnly)
      {
      Ctor(name, useUserOverride, readOnly);
      }

    CultureInfo::~CultureInfo()
      {
      }

    CultureInfo& CultureInfo::InvariantCulture() 
      {
      return _invariantCultureInfo;
      }

    void CultureInfo::Ctor(String name, bool useUserOverride, bool readOnly)
      {
      _isReadOnly = readOnly;
      _cultureID = InvariantCultureId;
      _parent_lcid = 0;
      _specific_lcid = 0;
      _datetime_index = 0;
      _number_index = 0;
      _default_calendar_type = 0;
      _useUserOverride = useUserOverride;
      _textinfo_data = nullptr;
      _constructed = true;

      if(name.Length() == 0)
        {
        // Short circuit the invariant culture
        ConstructInvariant();
        return;
        }

      Text::UTF8Encoding enc;
      String lw = name.ToLowerInvariant();
      ByteArray b = enc.GetBytes(lw);
      const CultureInfoNameEntry*  cine = ConstructFromName((const char*)b.ToConstPtr());
      if(cine == nullptr)
        {
        throw SystemException(L"CreateNotFoundException");
        // TODO: throw CreateNotFoundException(name);
        }
      if(!ConstructCulture(&culture_entries[cine->culture_entry_index]))
        {
        throw ArgumentException(L"culture");
        }
      }

    void CultureInfo::Ctor(uint32 culture, bool useUserOverride, bool readOnly)
      {
      _isReadOnly = readOnly;
      _cultureID = culture;
      _parent_lcid = 0;
      _specific_lcid = 0;
      _datetime_index = 0;
      _number_index = 0;
      _default_calendar_type = 0;
      _useUserOverride = useUserOverride;
      _textinfo_data = nullptr;
      _constructed = true;

      if(_cultureID == InvariantCultureId) 
        {
        // Short circuit the invariant culture
        ConstructInvariant();
        return;
        }

      if(!ConstructInternalLocaleFromLCID()) 
        {
        throw ArgumentException(L"culture");
        // TODO : throw new CultureNotFoundException ("culture",  String.Format ("Culture ID {0} (0x{0:X4}) is not a " + supported culture.", culture));
        }
      }

    //-------------------------------------------------------------------------
    //#pragma warning (disable:4296)
    void CultureInfo::ConstructInvariant()
      {
      // NumberFormatInfo defaults to the invariant data
      _numInfo.Set(NumberFormatInfo::InvariantInfo());

      if(!_isReadOnly)
        {
        if(_numInfo.Get() != nullptr)
          _numInfo.Set( new NumberFormatInfo((*_numInfo.Get())) );
        }

      // TODO : textInfo = CreateTextInfo(read_only);

      _englishname = L"Invariant Language (Invariant Country)";
      _nativename = _englishname;
      _iso3lang = L"IVL";
      _iso2lang = L"iv";
      _win3lang = L"IVL";
      // TODO : default_calendar_type = 1 << CalendarTypeBits;
      }

    bool CultureInfo::ConstructInternalLocaleFromLCID()
      {
      const CultureInfoEntry* ci = CultureInfoEntryFromLCID(_cultureID);
      if(ci == nullptr)
        return false;
      return ConstructCulture(ci);
      }

    bool CultureInfo::ConstructCulture(const CultureInfoEntry *ci)
      {
      _cultureID = ci->lcid;
      _name = MakeLocaleString(ci->name);
      _englishname = MakeLocaleString(ci->englishname);
      _nativename = MakeLocaleString(ci->nativename);
      _win3lang = MakeLocaleString(ci->win3lang);
      _iso3lang = MakeLocaleString(ci->iso3lang);
      _iso2lang = MakeLocaleString(ci->iso2lang);

      //// It's null for neutral cultures
      //if (ci->territory > 0)
      //  MONO_OBJECT_SETREF (this, territory, mono_string_new (domain, idx2string (ci->territory)));
      //MONO_OBJECT_SETREF (this, native_calendar_names, create_names_array_idx (ci->native_calendar_names, NUM_CALENDARS));
      _parent_lcid = ci->parent_lcid;
      _datetime_index = ci->datetime_format_index;
      _number_index = ci->number_format_index;
      _default_calendar_type = ci->calendar_type;
      _textinfo_data = (void*)&ci->text_info;

      return true;
      }

    Globalization::TextInfo* CultureInfo::CreateTextInfo()
      {
      return new Globalization::TextInfo(this, _cultureID, _textinfo_data, _isReadOnly);
      }
    //#pragma warning (default:4296)
    //-------------------------------------------------------------------------

    CultureInfo& CultureInfo::CurrentCulture()
      {
      return Threading::Thread::CurrentThread().CurrentCulture();
      }
    bool CultureInfo::IsReadOnly()
      {
      return _isReadOnly;
      }
    String CultureInfo::Name()
      {
      return _name;
      }
    int CultureInfo::LCID()
      {
      return _cultureID;
      }
    Object* CultureInfo::GetFormat(Object* type)
      {
      Object* format = nullptr;
      NumberFormatInfo nfi;

      if(Object::IsInstance(*type, nfi))
        format = (Object*)NumberFormat();
      // TODO: else if ( formatType == typeof(DateTimeFormatInfo) )
      //format = DateTimeFormat;

      return format;
      }
    CultureInfo* CultureInfo::ConstructCurrentCulture()
      {
      return new CultureInfo(::GetThreadLocale(), false, false);
      }
    NumberFormatInfo* CultureInfo::NumberFormat()
      {
      //if(!_constructed)
      //TODO : Construct ();
      // TODO : CheckNeutral ();
      //if(_numInfo.Get() == nullptr)
      //{
      //lock (this)
      //{
      if(_numInfo.Get() == nullptr)
        {
        _numInfo.Set(new NumberFormatInfo(_number_index, _isReadOnly));
        }
      //}
      //}

      return _numInfo.Get();
      }

    CompareInfo& CultureInfo::CompareInfo()
      {
      //if(_compareInfo==null) 
      //{
      //if (!constructed)
      //Construct ();

      //lock (this) 
      //{
      if(_compareInfo.Get() == nullptr) 
        {
        _compareInfo.Set(new Globalization::CompareInfo(*this));
        }
      //}
      //}

      return (*_compareInfo.Get());
      }

    TextInfo& CultureInfo::TextInfo()
      {
      if(_textInfo.Get() == nullptr)
        {
        //if(!_constructed) 
        // TODO : Construct();
        //lock (this)
        //{
        if(_textInfo.Get() == nullptr)
          {
          _textInfo.Set(CreateTextInfo());
          }
        //}
        }
      return(*_textInfo.Get());
      }
    }
  }
