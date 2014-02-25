// Perch Lake Computer System

#include "pch.h"
#include "System.Globalization.CultureInfo.h"
#include "System.Exception.h"
#include "System.Threading.Thread.h"
#include "LocaleStrings.h"

namespace System
  {
  namespace Globalization
    {
    CultureInfo::CultureInfo(uint32 culture, bool useUserOverride)
      :_isReadOnly(false)
      ,_cultureID(culture)
      ,_parent_lcid(0)
      ,_datetime_index(0)
      ,_number_index(0)
      ,_default_calendar_type(0)
      ,_useUserOverride(useUserOverride)
      ,_numInfo()
      ,_textInfo()
      ,_name()
      ,_englishname()
      ,_nativename()
      ,_compareInfo()
      ,_textinfo_data(nullptr)
      ,_constructed(true)
      {
      Init();
      }
    CultureInfo::CultureInfo(uint32 culture, bool useUserOverride, bool readOnly)
      :_isReadOnly(readOnly)
      ,_cultureID(culture)
      ,_parent_lcid(0)
      ,_datetime_index(0)
      ,_number_index(0)
      ,_default_calendar_type(0)
      ,_useUserOverride(useUserOverride)
      ,_numInfo()
      ,_textInfo()
      ,_name()
      ,_englishname()
      ,_nativename()
      ,_compareInfo()
      ,_textinfo_data(nullptr)
      ,_constructed(true)
      {
      Init();
      }

    CultureInfo::~CultureInfo()
      {
      }

    //-------------------------------------------------------------------------
#pragma warning (disable:4296)
    void CultureInfo::Init()
      {
      if(_cultureID < 0)
        throw ArgumentOutOfRangeException(L"culture", L"Positive number required.");

      if(_cultureID == InvariantCultureId) 
        {
        // Short circuit the invariant culture
        ConstructInvariant();
        return;
        }

      if(!construct_internal_locale_from_lcid()) 
        {
        throw ArgumentException(L"culture");
        // TODO : throw new CultureNotFoundException ("culture",  String.Format ("Culture ID {0} (0x{0:X4}) is not a " + supported culture.", culture));
        }
      }

    void CultureInfo::ConstructInvariant()
      {
      // NumberFormatInfo defaults to the invariant data
      // TODO : numInfo=NumberFormatInfo.InvariantInfo;

      if(!_isReadOnly)
        {
        // TODO : numInfo = (NumberFormatInfo) numInfo.Clone ();
        }

      // TODO : textInfo = CreateTextInfo(read_only);

      _englishname = L"Invariant Language (Invariant Country)";
      _nativename = _englishname;
      //iso3lang="IVL";
      //iso2lang="iv";
      //win3lang="IVL";
      // TODO : default_calendar_type = 1 << CalendarTypeBits;
      }

    bool CultureInfo::construct_internal_locale_from_lcid()
      {
      const CultureInfoEntry* ci = culture_info_entry_from_lcid(_cultureID);
      if(ci == nullptr)
        return false;
      return construct_culture(ci);
      }

    bool CultureInfo::construct_culture(const CultureInfoEntry *ci)
      {
      _cultureID = ci->lcid;
      _name = MakeLocaleString(ci->name);
      _englishname = MakeLocaleString(ci->englishname);
      _nativename = MakeLocaleString(ci->nativename);
      //MONO_OBJECT_SETREF (this, win3lang, mono_string_new (domain, idx2string (ci->win3lang)));
      //MONO_OBJECT_SETREF (this, iso3lang, mono_string_new (domain, idx2string (ci->iso3lang)));
      //MONO_OBJECT_SETREF (this, iso2lang, mono_string_new (domain, idx2string (ci->iso2lang)));

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
#pragma warning (default:4296)
    //-------------------------------------------------------------------------

    CultureInfo& CultureInfo::CurrentCulture()
      {
      return Threading::Thread::CurrentThread().CurrentCulture();
      }
    String CultureInfo::Name()
      {
      return _name;
      }
    int CultureInfo::LCID()
      {
      return _cultureID;
      }
    Object CultureInfo::GetFormat()
      {
      throw SystemException(L"Not Implemented");
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
