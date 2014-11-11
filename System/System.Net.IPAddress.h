#pragma once
#include "System.Net.Sockets.AddressFamily.h"

namespace System
  {
  namespace Net
    {
    class IPAddress;
    typedef SharedPtr<IPAddress> GCIPAddress;
    class IPAddress : public Object
      {
      private:
        int64                  _address;
        Sockets::AddressFamily _family;
      private:
        static GCIPAddress any;
        static GCIPAddress broadcast;
        static GCIPAddress loopback;
        static GCIPAddress none;
      public:
        IPAddress(int64 newAddress = 0);
        IPAddress(const IPAddress& /*address*/);
        ~IPAddress();
        IPAddress& operator=(const IPAddress& /*address*/);
        Sockets::AddressFamily AddressFamily();
        int64 InternalIPv4Address(){ return _address; }
        virtual uint32 GetHashCode() override;
        virtual bool Equals(Object* obj) override;
        virtual String ToString() override;
        static IPAddress* Parse(String ipString);
        static bool TryParse(String ipString, IPAddress** address);
        static IPAddress& Any();
        static IPAddress& Broadcast();
        static IPAddress& Loopback();
        static IPAddress& None();
      private:
        static IPAddress* ParseIPV4(String ip);
        static String ToString(int64 addr);
      };
    }
  }
