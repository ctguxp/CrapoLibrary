// Perch Lake Computer System

#pragma once
#include "System.Text.DecoderFallback.h"

namespace System
  {
  namespace Text
    {
    class DecoderReplacementFallback : public DecoderFallback
      {
      public:
        DecoderReplacementFallback(String replacement = L"?");
        ~DecoderReplacementFallback();
        String& DefaultString();
        virtual int MaxCharCount() override;
        virtual DecoderFallbackBuffer* CreateFallbackBuffer() override;
        // TODO: Need Object overrides.
      private:
        String _replacement;
      };
    }
  }