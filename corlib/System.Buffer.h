// Perch Lake Computer System

#pragma once
#include "System.Array.h"
#include "System.Exception.h"

namespace System
  {
  class CRAPOCOREDLL_API Buffer
    {
    public:
      Buffer();
      ~Buffer();

      template<class T>
      static bool BlockCopy(Array<T>& src, int src_offset, Array<T>& dest, int dest_offset, int count)
        {
        if(src.IsNull())
          {
          throw ArgumentNullException(L"src");
          }
        if(dest.IsNull())
          {
          throw ArgumentNullException(L"dest");
          }
        if( (src_offset > (int)src.Length() - count) || (dest_offset > (int)dest.Length() - count) )
          return false;

        T *src_buf = src.ToPtr() + src_offset;
        T *dest_buf = dest.ToPtr() + dest_offset;

        if(src.ToConstPtr() != dest.ToConstPtr())
          memcpy (dest_buf, src_buf, count);
        else
          memmove(dest_buf, src_buf, count);

        return true;
        }
    };
  }

