#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(UnitTestSingle)
    {
    public:

      TEST_METHOD(ToString_Defaults)
        {
        Single i(254.9f);
        // everything defaults to "G"
        String def = i.ToString(L"G");
        Assert::AreEqual((cstring)def, (cstring)i.ToString(), L"ToString()");
        Assert::AreEqual ((cstring)def, (cstring)i.ToString(nullptr), L"ToString((IFormatProvider)null)");
        Assert::AreEqual ((cstring)def, (cstring)i.ToString(String::Empty()), L"ToString(empty)");
        Assert::AreEqual ((cstring)def, (cstring)i.ToString(String::Empty(), nullptr), L"ToString(empty,null)");
        Assert::AreEqual(L"254.9", (cstring)def, L"ToString(G)");
        }

    };
  }