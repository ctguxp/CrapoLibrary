#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(DictionaryTest)
    {
    public:

      TEST_METHOD(Dictionary_Contains)
        {
        using namespace Collections;
        Generic::Dictionary<Int32, Int32> d;
        d.Add(1, 2);
        Assert::IsTrue(d.ContainsKey(1));
        Assert::IsFalse(d.ContainsKey(2));
        Assert::IsFalse(d.ContainsKey(L'x'));
        }

      TEST_METHOD(AddTest)
        {
        using namespace Collections;
        Generic::Dictionary<String, String> d;
        String key(L"key1");
        d.Add(key, L"value");
        String data;
        d.TryGetValue(key, data);
        Assert::AreEqual(L"value", (cstring)data, false, L"Add failed!");
        }

    };
  }