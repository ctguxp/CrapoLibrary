#include "pch.h"
#include "System.Net.Sockets.WinSock2.h"

namespace System
  {
  using namespace Runtime;
  namespace Net
    {
    namespace Sockets
      {
      AutoPtr<InteropServices::DllImport> WinSock2::_winsock2Dll(new InteropServices::DllImport(L"ws2_32.dll"));

      WinSock2::WinSock2()
        {
        WS2Startup();
        }
      WinSock2::~WinSock2()
        {
        WS2Cleanup();
        }
      void WinSock2::WS2Startup()
        {
        WORD wVersionRequested;
        WSADATA wsaData;

        wVersionRequested = MAKEWORD(2, 2);
        Startup startup;
        _winsock2Dll->GetFunction(L"WSAStartup", startup);
        int err = startup(wVersionRequested, &wsaData);
        if(err)
          throw SystemException(L"Failed to startup winsock2");
        }
      void WinSock2::WS2Cleanup()
        {
        Cleanup cleanup;
        _winsock2Dll->GetFunction(L"WSACleanup", cleanup);
        cleanup();
        }
      int WinSock2::WS2Bind(SOCKET s, const sockaddr* name, int namelen)
        {
        Bind binder;
        _winsock2Dll->GetFunction(L"bind", binder);
        return binder(s, name, namelen);
        }
      uint16 WinSock2::WS2Htons(uint16 hostshort)
        {
        Htons h;
        _winsock2Dll->GetFunction(L"htons", h);
        return h(hostshort);
        }
      uint32 WinSock2::WS2Htonl(uint32 hostlong)
        {
        Htonl h;
        _winsock2Dll->GetFunction(L"htonl", h);
        return h(hostlong);
        }
      int WinSock2::WS2Connect(SOCKET s, const sockaddr* name, int namelen)
        {
        Connect con;
        _winsock2Dll->GetFunction(L"connect", con);
        return con(s, name, namelen);
        }
      int WinSock2::WS2Close(SOCKET s)
        {
        Close closeSock;
        _winsock2Dll->GetFunction(L"closesocket", closeSock);
        return closeSock(s);
        }
      SOCKET WinSock2::WS2Socket(int af, int type, int protocol)
        {
        Socket sock;
        _winsock2Dll->GetFunction(L"socket", sock);
        return sock(af, type, protocol);
        }
      int WinSock2::WS2GetLastError()
        {
        GetLastError lasterr;
        _winsock2Dll->GetFunction(L"WSAGetLastError", lasterr);
        return lasterr();
        }
      int WinSock2::WS2Listen(SOCKET s, int backlog)
        {
        Listen listener;
        _winsock2Dll->GetFunction(L"listen", listener);
        return listener(s, backlog);
        }
      }
    }
  }
