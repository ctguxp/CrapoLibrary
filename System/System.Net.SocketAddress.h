#pragma once
#include "System.Net.Sockets.AddressFamily.h"

namespace System
  {
  namespace Net
    {
    class SocketAddress
      {
      private:
        ByteArray _data;
      public:
        SocketAddress(Sockets::AddressFamily family, int32 size = 32);
        SocketAddress(const SocketAddress& sa);
        ~SocketAddress();
        SocketAddress& operator=(const SocketAddress& sa);
        byte Get(int32 offset);
        void Set(int32 offset, byte value);
        ByteArray Data();
      };
    typedef SharedPtr<SocketAddress> GCSocketAddress;
    }
  }
