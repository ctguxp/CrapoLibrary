#include "pch.h"
#include "System.Net.Sockets.Socket.h"
#include "System.Net.Sockets.WinSock2.h"
#include "System.Net.SocketAddress.h"
#include "System.Net.IPEndPoint.h"

namespace System
  {
  namespace Net
    {
    namespace Sockets
      {
      Socket::Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType)
        :_adddressFamily(addressFamily)
        ,_socketType(socketType)
        ,_protocolType(protocolType)
        ,_connected(false)
        ,_isBound(false)
        ,_isListening(false)
        ,_handle(INVALID_SOCKET)
        {
        int num = 0;
        SocketInternal(_adddressFamily, _socketType, protocolType, num);
        }

      Socket::~Socket()
        {
        if(_handle != INVALID_SOCKET)
          WinSock2::WS2Close(_handle);
        }

      void Socket::SocketInternal(AddressFamily family, SocketType type, ProtocolType proto, int& error)
        {
        _handle = WinSock2::WS2Socket((int)family, (int)type, (int)proto);
        if(_handle == INVALID_SOCKET)
          {
          error = WinSock2::WS2GetLastError();
          }
        }

      void Socket::Bind(EndPoint& local_end) 
        {
        /* if (disposed && closed)
        throw new ObjectDisposedException (GetType ().ToString ());*/

        /* if(local_end == null)
        throw new ArgumentNullException("local_end");*/

        int error = 0;

        BindInternal(_handle, local_end.Serialize(), error);
        if(error != 0)
          //throw new SocketException (error);
            throw SystemException(L"Socket bind error");
        if(error == 0)
          _isBound = true;

        //seed_endpoint = local_end;
        }

      void Socket::Connect(GCIPAddress& address, int port)
        {
        IPEndPoint ep(address, port);
        Connect(ep);
        }

      void Socket::Connect(EndPoint& remoteEP)
        {
        GCSocketAddress serial;

        /*if (disposed && closed)
        throw new ObjectDisposedException (GetType ().ToString ());

        if (remoteEP == null)
        throw new ArgumentNullException ("remoteEP");*/

        //IPEndPoint ep = remoteEP as IPEndPoint;
        IPEndPoint& ep = static_cast<IPEndPoint&>(remoteEP);
        if(&ep != nullptr && _socketType != SocketType::Dgram) /* Dgram uses Any to 'disconnect' */
          if(ep.Address().Equals(&IPAddress::Any()) /*|| ep.Address.Equals(IPAddress::IPv6Any())*/)
            //throw SocketException ((int) SocketError.AddressNotAvailable);
              throw SystemException(L"Socket address not available");

        if(_isListening)
          //throw new InvalidOperationException ();
            throw SystemException(L"Invalid operation");
        serial = remoteEP.Serialize();

        int32 error = 0;

        //try 
        //{
        //RegisterForBlockingSyscall();
        ConnectInternal(_handle, serial, error);
        //} 
        //finally 
        //{
        // UnRegisterForBlockingSyscall ();
        //}

        //if(error == 0 || error == 10035)
        //seed_endpoint = remoteEP; // Keep the ep around for non-blocking sockets

        if(error != 0) 
          {
          //if(_closed)
          //error = SOCKET_CLOSED;
          //throw new SocketException (error);
          throw SystemException(L"Socket closed???");
          }

        if(_socketType == SocketType::Dgram && (ep.Address().Equals(&IPAddress::Any()) /*|| ep.Address.Equals(IPAddress::IPv6Any))*/))
          _connected = false;
        else
          _connected = true;

        _isBound = true;
        }

      void Socket::Listen(int32 backlog)
        {
        /*if(disposed && closed)
        throw new ObjectDisposedException (GetType ().ToString ());*/

        /*if (!isbound)
        throw new SocketException ((int)SocketError.InvalidArgument);*/

        int error = 0;
        ListenInternal(_handle, backlog, error);

        if(error != 0)
          //throw new SocketException (error);
            throw SystemException(L"Socket listen error");

        _isListening = true;
        }

      void Socket::BindInternal(uintptr sock, GCSocketAddress sa, int& error)
        {
        sockaddr* st = nullptr;
        sizet sa_size = 0;
        ByteArray data = sa->Data();
        int32 len = (int32)data.Length();
        if(len < 2)
          throw SystemException();
        int family = (data[0] + (data[1] << 8));
        if(family == AF_INET)
          {
          sockaddr_in s;
          s.sin_family = AF_INET;
          uint16 port = (data[2] << 8) + data[3];
          s.sin_port = WinSock2::WS2Htons(port);
          uint32 address = (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + data[7];
          s.sin_addr.s_addr = WinSock2::WS2Htonl(address);
          sa_size = sizeof(sockaddr_in);
          st = (sockaddr*)&s; 
          }
        int32 ret = WinSock2::WS2Bind(sock, st, (int32)sa_size);
        if(ret != SOCKET_ERROR)
          error = WinSock2::WS2GetLastError();
        }

      void Socket::ConnectInternal(uintptr sock, GCSocketAddress sa, int& error)
        {
        sockaddr* st = nullptr;
        sizet sa_size = 0;
        ByteArray data = sa->Data();
        int32 len = (int32)data.Length();
        if(len < 2)
          throw SystemException();
        int family = (data[0] + (data[1] << 8));
        if(family == AF_INET)
          {
          sockaddr_in s;
          s.sin_family = AF_INET;
          uint16 port = (data[2] << 8) + data[3];
          s.sin_port = WinSock2::WS2Htons(port);
          uint32 address = (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + data[7];
          s.sin_addr.s_addr = WinSock2::WS2Htonl(address);
          sa_size = sizeof(sockaddr_in);
          st = (sockaddr*)&s; 
          }
        int32 ret = WinSock2::WS2Connect(sock, st, (int32)sa_size);
        if(ret != SOCKET_ERROR)
          error = WinSock2::WS2GetLastError();
        }

      void Socket::ListenInternal(uintptr sock, int32 backlog, int32& error)
        {
        error = 0;
        int32 ret = WinSock2::WS2Listen(sock, backlog);
        if(ret != SOCKET_ERROR)
          error = WinSock2::WS2GetLastError();
        }

      }
    }
  }