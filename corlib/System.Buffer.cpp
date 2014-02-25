#include "pch.h"
#include "System.Buffer.h"
#include "System.Exception.h"
#include "System.Array.hpp"

namespace System
  {
  Buffer::Buffer()
    {
    }
  Buffer::~Buffer()
    {
    }
  bool Buffer::BlockCopy(ByteArray& src, int src_offset, ByteArray& dest, int dest_offset, int count)
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

    byte *src_buf = src.ToPtr() + src_offset;
    byte *dest_buf = dest.ToPtr() + dest_offset;

    if(src.ToConstPtr() != dest.ToConstPtr())
      memcpy (dest_buf, src_buf, count);
    else
      memmove(dest_buf, src_buf, count);

    return true;
    }
  }
