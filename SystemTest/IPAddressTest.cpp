#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace SystemTest
  {
  using namespace Net;
  TEST_CLASS(IPAddressTest)
    {
    public:

      TEST_METHOD(ToStringV4)
        {
        GCIPAddress ip(IPAddress::Parse(L"192.168.1.1"));
        Assert::AreEqual(L"192.168.1.1", ip->ToString (), L"#1");
        Assert::AreEqual(L"0.0.0.0", IPAddress::Any().ToString(), L"#2");
        Assert::AreEqual(L"255.255.255.255", IPAddress::Broadcast().ToString (), "#3");
        Assert::AreEqual(L"127.0.0.1", IPAddress::Loopback().ToString (), L"#4");
        Assert::AreEqual(L"255.255.255.255", IPAddress::None().ToString (), L"#5");
        }

    };
  }