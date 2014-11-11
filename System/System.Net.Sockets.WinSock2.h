#pragma once
#include "System.Runtime.InteropServices.DllImport.h"

namespace System
  {
  namespace Net
    {
    namespace Sockets
      {
      class WinSock2
        {
        private:
          static AutoPtr<Runtime::InteropServices::DllImport> _winsock2Dll;
        public:
          WinSock2();
          ~WinSock2();
          static int WS2GetLastError();
          static int WS2Bind(SOCKET /*s*/, const sockaddr* /*name*/, int /*namelen*/);
          static int WS2Close(SOCKET /*s*/);
          static int WS2Connect(SOCKET /*s*/, const sockaddr* /*name*/, int /*namelen*/);  
          static uint16 WS2Htons(uint16 /*hostshort*/);
          static uint32 WS2Htonl(uint32 /*hostlong*/);
          static SOCKET WS2Socket(int /*af*/, int /*type*/, int /*protocol*/);
          static int WS2Listen(SOCKET /*s*/, int /*backlog*/);
        private:
          void WS2Startup();
          void WS2Cleanup();
        private:
          typedef int (FAR PASCAL* Startup)(WORD /*wVersionRequested*/, LPWSADATA /*lpWSAData*/);
          typedef int (FAR PASCAL* Cleanup)();
          typedef int (FAR PASCAL* GetLastError)();
          typedef SOCKET (FAR PASCAL* Socket)(int /*af*/, int /*type*/, int /*protocol*/);
          typedef int (FAR PASCAL* Bind)(SOCKET /*s*/, const struct sockaddr* /*name*/, int /*namelen*/);
          typedef int (FAR PASCAL* Close)(SOCKET /*s*/);
          typedef int (FAR PASCAL* Connect)(SOCKET /*s*/, const struct sockaddr* /*name*/, int /*namelen*/);
          typedef uint16 (FAR PASCAL* Htons)(uint16 /*hostshort*/);
          typedef uint32 (FAR PASCAL* Htonl)(uint32 /*hostlong*/);
          typedef int (FAR PASCAL* Listen)(SOCKET /*s*/, int /*backlog*/);
        };
      }
    }
  }

