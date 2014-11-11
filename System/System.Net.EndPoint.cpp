#include "pch.h"
#include "System.Net.EndPoint.h"

namespace System
  {
  namespace Net
    {
    EndPoint::EndPoint()
      {
      }

    EndPoint::~EndPoint()
      {
      }

    Sockets::AddressFamily EndPoint::AddressFamily()
      {
      throw NotImplementedException();
      }

    EndPoint* EndPoint::Create(SocketAddress&)
      {
      throw NotImplementedException();
      }

    GCSocketAddress EndPoint::Serialize()
      {
      throw NotImplementedException();
      }

    }
  }
