#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(UnitTestGenericList)
    {
    public:

      TEST_METHOD(ConstructWithSizeTest)
        {
        using namespace Collections::Generic;

        List<Object*> l_1(1);
        List<Object> l_2(50);
        List<Object> l_3(0);

        Assert::AreEqual<uintptr>(1, l_1.Capacity());
        Assert::AreEqual<uintptr>(50, l_2.Capacity());
        Assert::AreEqual<uintptr>(0, l_3.Capacity());
        }

      TEST_METHOD(AddTest)
        {
        using namespace Collections::Generic;

        List<int> list1;
        uintptr count = list1.Count();
        list1.Add (-1);
        Assert::AreEqual<uintptr>(count + 1,list1.Count());
        Assert::AreEqual<int>(-1, list1[list1.Count() - 1]);
        }

      TEST_METHOD(ToArrayTest)
        {
        using namespace Collections::Generic;

        List<int> list1;
        list1.Add(-1);
        IntArray copy = list1.ToArray();

        // TODO : Assert.IsFalse (ReferenceEquals (copiedContents, _list1_contents));

        Assert::AreEqual<uintptr>(list1.Count(), copy.Length());
        Assert::AreEqual<int>(list1[0], copy[0]);
        }
    };
  }