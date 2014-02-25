#pragma once

namespace System
  {
  namespace IO
    {
    enum class CrapoFileType
      {
      Unknown=0x0000,
      Disk=0x0001,
      Char=0x0002,
      Pipe=0x0003,
      Remote=0x8000,
      };
    }
  }
