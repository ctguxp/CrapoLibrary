#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  Collections::Stack stack1;
  Collections::Stack stackInt;

  TEST_CLASS(UnitTestStack)
    {
    public:

      TEST_METHOD(TestCount)
        {
        using namespace Collections;			
        Stack stackTest;
        stackTest.Push(GCObject(new Int32(50)));
        stackTest.Push(GCObject(new Int32(5)));
        stackTest.Push(GCObject(new Int32(0)));
        stackTest.Push(GCObject(new Int32(50)));

        Assert::AreEqual<sizet>(stackTest.Count(), 4);
        }

      TEST_METHOD(TestContains)
        {
        using namespace Collections;	
        GCObject toLocate(new String(L"test"));

        stackInt.Push(toLocate);

        stackInt.Push(GCObject(new String(L"chaff")));

        stackInt.Push(GCObject());

        Assert::IsTrue(stackInt.Contains(toLocate.Get()));

        Assert::IsTrue(stackInt.Contains(nullptr), L"must contain null");

        GCObject i = stackInt.Pop();

        i = stackInt.Pop();
        i.Reset();

        Assert::IsTrue(stackInt.Contains(toLocate.Get()));

        i = stackInt.Pop();
        i.Reset();

        Assert::IsTrue(!stackInt.Contains(toLocate.Get()));

        stackInt.Push(GCObject());
        Assert::IsTrue (stackInt.Contains(nullptr));
        stackInt.Pop();
        Assert::IsTrue(!stackInt.Contains(nullptr));
        }
    };
  }