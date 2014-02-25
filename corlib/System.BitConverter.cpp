#include "pch.h"
#include "System.BitConverter.h"

namespace System
  {
  
  bool BitConverter::IsLittleEndian = BitConverter::AmILittleEndian();

  BitConverter::BitConverter()
    {
    }
  BitConverter::~BitConverter()
    {
    }
  bool BitConverter::AmILittleEndian()
		{
			// binary representations of 1.0:
			// big endian:            3f f0 00 00 00 00 00 00
			// little endian:         00 00 00 00 00 00 f0 3f
			double d = 1.0;
			byte *b = (byte*)&d;
			return (b [0] == 0);
		}
  }
