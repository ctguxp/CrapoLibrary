#pragma once
#include "System.Net.EndPoint.h"
#include "System.Net.IPAddress.h"

namespace System
  {
  namespace Net
    {
    class IPAddress;
    class IPEndPoint : public EndPoint
      {
      private:
        int32       _port;
        GCIPAddress _address;
      public:
        IPEndPoint(GCIPAddress& address, int32 port);
        ~IPEndPoint();
        virtual GCSocketAddress Serialize() override;
        IPAddress& Address() { return *_address; }
      };
    }
  }

