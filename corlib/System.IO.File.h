// Perch Lake Computer System

#pragma once
#include "System.String.h"

namespace System
  {
  namespace IO
    {
    class FileStream;
    class CRAPOCOREDLL_API File
      {
      public:
        File();
        ~File();

        // Methods
        static FileStream* Create(String& path);
        static FileStream* Create(String& path, int bufferSize);
        static void Delete(String& path);
        static bool Exists(String& path);
        static FileStream* OpenRead(String path);
      };
    }
  }
