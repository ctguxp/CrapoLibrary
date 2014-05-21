#include "pch.h"
#include "System.IO.Stream.h"
#include "System.Array.hpp"

namespace System
  {
  namespace IO
    {
    Stream::Stream()
      {
      }
    Stream::~Stream()
      {
      }
    void Stream::Close()
      {
      }
    int Stream::ReadByte()
		{
			ByteArray buffer(1);
			if (Read (buffer, 0, 1) > 0)
				return buffer[0];
			
			return -1;
		}
    void Stream::WriteByte(byte value)
      {
      ByteArray buffer(1);
      buffer[0] = value;
      Write(buffer, 0, 1);
      }
    }
  }