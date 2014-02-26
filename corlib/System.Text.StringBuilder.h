// Perch Lake Computer System

#pragma once
#include "AutoPtr.h"
#include "System.String.h"
#include "System.Int32.h"

namespace System
  {
  namespace Text
    {
    class CRAPOCOREDLL_API StringBuilder : public System::Object
      {
      enum
        {
        constDefaultCapacity = 16
        };
      public:
        StringBuilder();
        StringBuilder(String, int startIndex = 0, int length = 0, int capacity = 0);
        StringBuilder(uint32, uint32 maxCapacity = Int32::MaxValue);
        ~StringBuilder();
        /*const wchar_t& operator [] (uint32 idx) const;
        wchar_t& operator [] (uint32 idx);*/
        int Capacity();
        int Length();
        void Length(uint32 value);
        uint32 MaxCapacity();
        
        StringBuilder& StringBuilder::Append(CharArray&);
        StringBuilder& Append(wchar_t); 
        StringBuilder& Append(wchar_t, int);
        StringBuilder& Append(const wchar_t*);
        StringBuilder& Append(System::String);
        StringBuilder& Append(System::String, int, int) ;
        virtual String ToString() override;
      private:
        void Initialize(String& value, int startIndex, int length, int capacity);
        void InternalEnsureCapacity(uint32); 
      private:
        uint32   _length;
        uint32   _maxCapacity;
        String   _cached_str;
        String   _str;
      };

    typedef AutoPtr<StringBuilder> GCStringBuilder;
    }
  }

