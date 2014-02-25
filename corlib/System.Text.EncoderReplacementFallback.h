// Perch Lake Computer System

#pragma once
#include "System.Text.EncoderFallback.h"

namespace System
  {
  namespace Text
    {
    class EncoderReplacementFallback : public EncoderFallback
      {
      public:
        EncoderReplacementFallback(String replacement = L"?");
        ~EncoderReplacementFallback();
        String& DefaultString();
        virtual int MaxCharCount() override;
        virtual EncoderFallbackBuffer* CreateFallbackBuffer() override;
        // TODO: Need Object overrides.
      private:
        String _replacement;
      };
    }
  }
