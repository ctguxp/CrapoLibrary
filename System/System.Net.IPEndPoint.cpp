#include "pch.h"
#include "System.Net.IPEndPoint.h"
#pragma warning (disable:4062)

namespace System
  {
  namespace Net
    {
    IPEndPoint::IPEndPoint(GCIPAddress& address, int32 port)
      :_port(port)
      ,_address()
      {
      if(address.Get() == nullptr)
        throw ArgumentNullException(L"address");

      _address = address;
      }

    IPEndPoint::~IPEndPoint()
      {
      }

    GCSocketAddress IPEndPoint::Serialize()
      {
      GCSocketAddress sockaddr;
      switch(_address->AddressFamily())
        {
        case Sockets::AddressFamily::InterNetwork:
          {
          // .net produces a 16 byte buffer, even though
          // only 8 bytes are used. I guess its just a
          // holdover from struct sockaddr padding.
          sockaddr.Reset(new SocketAddress(Sockets::AddressFamily::InterNetwork, 16));

          // bytes 2 and 3 store the port, the rest
          // stores the address
          sockaddr->Set(2, (byte)((_port>>8) & 0xff));
          sockaddr->Set(3, (byte)(_port & 0xff));
          int64 addr = _address->InternalIPv4Address();
          sockaddr->Set(4, (byte)(addr & 0xff));
          sockaddr->Set(5, (byte)((addr >> 8) & 0xff));
          sockaddr->Set(6, (byte)((addr >> 16) & 0xff));
          sockaddr->Set(7, (byte)((addr >> 24) & 0xff));
          }
          break;

          /*case Sockets::AddressFamily::InterNetworkV6:
          {
          sockaddr = new SocketAddress(AddressFamily.InterNetworkV6, 28);

          sockaddr [2] = (byte) ((port>>8) & 0xff);
          sockaddr [3] = (byte) (port & 0xff);

          byte[] addressBytes = address.GetAddressBytes();
          for(int i=0; i<16; i++)
          sockaddr[8+i] = addressBytes[i];

          sockaddr [24] = (byte) (address.ScopeId & 0xff);
          sockaddr [25] = (byte) ((address.ScopeId >> 8) & 0xff);
          sockaddr [26] = (byte) ((address.ScopeId >> 16) & 0xff);
          sockaddr [27] = (byte) ((address.ScopeId >> 24) & 0xff);
          }
          break;*/

        }
      return sockaddr;
      }
    }
  }
