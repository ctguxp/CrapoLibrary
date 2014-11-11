#pragma once
#include "System.Net.Sockets.AddressFamily.h"
#include "System.Net.SocketAddress.h"

namespace System
  {
  namespace Net
    {
    class EndPoint
      {
      public:
        virtual ~EndPoint();
        virtual Sockets::AddressFamily AddressFamily();
        virtual EndPoint* Create(SocketAddress& sockaddress);
        virtual GCSocketAddress Serialize();
      protected:
        EndPoint();
      };
    }
  }
