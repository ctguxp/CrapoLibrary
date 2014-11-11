#include "pch.h"
#include "System.Net.SocketAddress.h"

namespace System
  {
  namespace Net
    {
    SocketAddress::SocketAddress(Sockets::AddressFamily family, int32 size)
      :_data()
      {
      if(size < 2)
        {
        throw ArgumentOutOfRangeException(L"size is too small");
        }

      _data.Length(size);
      _data[0]=(byte)family;
      _data[1]=(byte)((int32) family >> 8);
      }

    SocketAddress::SocketAddress(const SocketAddress& sa)
      :_data(sa._data)
      {
      }

    SocketAddress::~SocketAddress()
      {
      }

    SocketAddress& SocketAddress::operator=(const SocketAddress& sa)
      {
      if(this == &sa)
        return *this;
  
      _data = sa._data;
      return *this;
      }

    byte SocketAddress::Get(int32 offset)
      {
      return _data[offset];
      }
        
    void SocketAddress::Set(int32 offset, byte value)
      {
      _data[offset] = value;
      }

    ByteArray SocketAddress::Data()
      {
      return _data;
      }

    }
  }
