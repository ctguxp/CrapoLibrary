#include "pch.h"
#include "System.Net.IPAddress.h"

namespace System
  {
  namespace Net
    {

    GCIPAddress IPAddress::any(new IPAddress(0));
    GCIPAddress IPAddress::broadcast(IPAddress::Parse(L"255.255.255.255"));
    GCIPAddress IPAddress::loopback(IPAddress::Parse(L"127.0.0.1"));
    GCIPAddress IPAddress::none(IPAddress::Parse(L"255.255.255.255"));

    IPAddress::IPAddress(int64 newAddress)
      :_address(newAddress)
      ,_family(Sockets::AddressFamily::InterNetwork)
      {
      }

    IPAddress::IPAddress(const IPAddress& address)
      :_address(address._address)
      ,_family(address._family)
      {
      }

    IPAddress::~IPAddress()
      {
      }

    IPAddress& IPAddress::operator=(const IPAddress& address)
      {
      if(this == &address)
        return *this;

      _address = address._address;
      _family = address._family;
      return *this;
      }

    Sockets::AddressFamily IPAddress::AddressFamily() 
      {
      return _family;
      }

    IPAddress& IPAddress::Any()
      {
      return *any;
      }

    IPAddress& IPAddress::Broadcast()
      {
      return *broadcast;
      }

    IPAddress& IPAddress::Loopback()
      {
      return *loopback;
      }

    IPAddress& IPAddress::None()
      {
      return *none;
      }

    bool IPAddress::Equals(Object* obj)
      {
      IPAddress* otherAddr = static_cast<IPAddress*>(obj);
      if(otherAddr != nullptr)
        {
        if(AddressFamily() != otherAddr->AddressFamily())
          return false;

        //if(AddressFamily() == AddressFamily::InterNetwork)
        //{
        return _address == (*otherAddr)._address;
        //} 
        /*else 
        {
        ushort[] vals = otherAddr.m_Numbers;

        for(int i=0; i<8; i++)
        if(m_Numbers[i] != vals[i])
        return false;

        return true;
        }*/
        }
      return false;
      }

    uint32 IPAddress::GetHashCode()
      {
      //if(m_Family == AddressFamily.InterNetwork)
      return (int32)_address;
      /*else
      return Hash (((((int) m_Numbers[0]) << 16) + m_Numbers [1]), 
      ((((int) m_Numbers [2]) << 16) + m_Numbers [3]),
      ((((int) m_Numbers [4]) << 16) + m_Numbers [5]),
      ((((int) m_Numbers [6]) << 16) + m_Numbers [7]));*/
      }

    String IPAddress::ToString()
      {
      using namespace Sockets;
      if(_family == AddressFamily::InterNetwork)
        return ToString(_address);
      else
        {
        /*ushort[] numbers = m_Numbers.Clone() as ushort[];

        for(int i=0; i<numbers.Length; i++)
        numbers[i] = (ushort)NetworkToHostOrder((short)numbers[i]);

        IPv6Address v6 = new IPv6Address(numbers);
        v6.ScopeId = ScopeId;
        return v6.ToString();*/
        return String::Empty();
        }
      }

    IPAddress* IPAddress::Parse(String ipString)
      {
      IPAddress* ret = nullptr;
      if(TryParse(ipString, &ret))
        return ret;
      throw FormatException(L"An invalid IP address was specified.");
      }

    bool IPAddress::TryParse(String ipString, IPAddress** address)
      {
      /*if (ipString == null)
      throw new ArgumentNullException ("ipString");*/

      if((*address = ParseIPV4(ipString)) == nullptr)
        //if((address = ParseIPV6(ipString)) == nullptr)
          return false;
      return true;
      }

    IPAddress* IPAddress::ParseIPV4(String ip)
      {
      int32 pos = ip.IndexOf(L' ');
      if(pos != -1) 
        {
        /*string [] nets = ip.Substring (pos + 1).Split (new char [] {'.'});
        if (nets.Length > 0) 
        {
        string lastNet = nets [nets.Length - 1];
        if (lastNet.Length == 0)
        return null;
        foreach (char c in lastNet)
        if (!Uri.IsHexDigit (c))
        return null;
        }
        ip = ip.Substring (0, pos);*/
        }

      if(ip.Length() == 0 || ip[ip.Length() - 1] == '.')
        return nullptr;

      CharArray dot(1);
      dot[0] = L'.';
      StringArray ips = ip.Split(dot);
      if(ips.Length() > 4)
        return nullptr;

      // Make the number in network order
      try 
        {
        int64 a = 0;
        int64 val = 0;
        for(int32 i = 0; i < (int32)ips.Length(); i++) 
          {
          String subnet = ips[i];
          if((3 <= subnet.Length() && subnet.Length() <= 4) && (subnet[0] == L'0') && (subnet[1] == L'x' || subnet[1] == L'X')) 
            {
            //if(subnet.Length() == 3)
            //val = (byte)Uri::FromHex(subnet[2]);
            //else 
            //val = (byte)((Uri::FromHex(subnet[2]) << 4) | Uri::FromHex(subnet[3]));
            } 
          else if(subnet.Length() == 0)
            return nullptr;
          else if(subnet[0] == L'0') 
            {
            // octal
            val = 0;
            for(int j = 1; j < subnet.Length(); j++)
              {
              if(L'0' <= subnet[j] && subnet[j] <= L'7')
                val = (val << 3) + subnet[j] - L'0';
              else
                return nullptr;
              }
            }
          else 
            {
            if(!Int64::TryParse(subnet, Globalization::NumberStyles::None, nullptr, val))
              return nullptr;
            }

          if(i == ((int32)ips.Length() - 1)) 
            {
            if(i != 0  && val >= (256 << ((3 - i) * 8)))
              return nullptr;
            else if (val > 0x3fffffffe) // this is the last number that parses correctly with MS
              return nullptr;
            i = 3;
            } 
          else if(val >= 0x100)
            return nullptr;
          for(int j = 0; val > 0; j++, val /= 0x100)
            a |= (val & 0xFF) << ((i - j) << 3);
          }

        return new IPAddress(a);
        } 
      catch(Exception&) 
        {
        return nullptr;
        }
      }

    String IPAddress::ToString(int64 addr)
      {
      // addr is in network order
      String dot(L".");
      Int64 oct1(addr & 0xff);
      Int64 oct2((addr >> 8) & 0xff);
      Int64 oct3((addr >> 16) & 0xff);
      Int64 oct4((addr >> 24) & 0xff);
      return oct1.ToString() + dot + oct2.ToString() + dot + oct3.ToString() + dot + oct4.ToString();
      }

    }
  }
