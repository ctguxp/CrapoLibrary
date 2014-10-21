#pragma once
#include "System.Array.h"
#include "System.String.h"
#include "System.IFormatProvider.h"

namespace System
  {
  namespace IO
    {
    class CRAPOCOREDLL_API TextWriter
      {
      public:
        virtual ~TextWriter();
        virtual String& NewLine();
        virtual void NewLine(String& /*value*/);
        virtual void Write(bool /*value*/);
        virtual void Write(double /*value*/);
        virtual void Write(float /*value*/);
        virtual void Write(int32 /*value*/);
        virtual void Write(int64 /*value*/);
        virtual void Write(wchar_t /*value*/);
        virtual void Write(String /*value*/);
        virtual void Write(Object* /*value*/);
        virtual void Write(CharArray /*buffer*/);
        virtual void Write(CharArray /*buffer*/, int32 /*index*/, int32 /*count*/);
        virtual void WriteLine();
        virtual void WriteLine(bool /*value*/);
        virtual void WriteLine(wchar_t /*value*/);
        virtual void WriteLine(CharArray /*buffer*/);
      protected:
        TextWriter();
        TextWriter(IFormatProvider* /*formatProvider*/);
      protected:
        CharArray        _coreNewLine;
        String           _newLine;
        IFormatProvider* _internalFormatProvider;
      };
    }
  }
