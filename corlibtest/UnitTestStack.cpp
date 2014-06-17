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
        stackTest.Push(new Int32(50));
        stackTest.Push(new Int32(5));
        stackTest.Push(new Int32(0));
        stackTest.Push(new Int32(50));

        Assert::AreEqual<sizet>(stackTest.Count(), 4);
        }

      TEST_METHOD(TestContains)
        {
        using namespace Collections;	
        String* toLocate = new String(L"test");

        stackInt.Push(toLocate);

        stackInt.Push(new String(L"chaff"));

        stackInt.Push(nullptr);

        Assert::IsTrue(stackInt.Contains(toLocate));

        Assert::IsTrue(stackInt.Contains(nullptr), L"must contain null");

        Object* i = stackInt.Pop();

        i = stackInt.Pop();
        delete i;
        i = nullptr;

        Assert::IsTrue (stackInt.Contains(toLocate));

        i = stackInt.Pop();
        delete i;
        i = nullptr;

        Assert::IsTrue(!stackInt.Contains(toLocate));

        stackInt.Push(nullptr);
        Assert::IsTrue (stackInt.Contains(nullptr));
        stackInt.Pop();
        Assert::IsTrue(!stackInt.Contains(nullptr));
        }
    };
  }