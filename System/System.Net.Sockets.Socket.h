#pragma once
#include "System.Net.Sockets.AddressFamily.h"
#include "System.Net.Sockets.SocketType.h"
#include "System.Net.Sockets.ProtocolType.h"
#include "System.Net.SocketAddress.h"
#include "System.Net.IPAddress.h"

namespace System
  {
  namespace Net
    {
    class EndPoint;
    namespace Sockets
      {
      class Socket
        {
        private:
          AddressFamily _adddressFamily;
          SocketType    _socketType;
          ProtocolType  _protocolType;
          bool          _connected;
          bool          _isBound;
          bool          _isListening;
          uintptr       _handle;
        public:
          Socket(AddressFamily /*addressFamily*/, SocketType /*socketType*/, ProtocolType /*protocolType*/);
          ~Socket();
          void Bind(EndPoint& /*local_end*/);
          void Connect(GCIPAddress& address, int port);
          void Connect(EndPoint& /*remoteEP*/);
          void Listen(int32 /*backlog*/);
          bool Connected() { return _connected; }
        private:
          void SocketInternal(AddressFamily /*family*/, SocketType /*type*/, ProtocolType /*proto*/, int& /*error*/);
          static void BindInternal(uintptr /*sock*/, GCSocketAddress /*sa*/, int& /*error*/);
          static void ConnectInternal(uintptr sock, GCSocketAddress sa, int& error);
          static void ListenInternal(uintptr /*sock*/, int32 /*backlog*/, int32& /*error*/);
        };
      }
    }
  }

