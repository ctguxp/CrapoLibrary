#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace SystemTest
  {
  using namespace Net;
  TEST_CLASS(SocketTest)
    {
    private:
      Sockets::WinSock2 _winSock;

    public:
      SocketTest()
        :_winSock()
        {
        }    

      TEST_METHOD(ConnectAddressPort)
        {
        using namespace Sockets;
        Socket sock(AddressFamily::InterNetwork,
          SocketType::Stream,
          ProtocolType::Tcp);
        Socket listen(AddressFamily::InterNetwork,
          SocketType::Stream,
          ProtocolType::Tcp);
        GCIPAddress ip(new IPAddress(IPAddress::Loopback()));
        IPEndPoint ep(ip, 1249);

        listen.Bind(ep);
        listen.Listen(1);

        sock.Connect(ip, 1249);

        Assert::AreEqual(true, sock.Connected(), L"ConnectAddressPort #1");
        }

    };
  }