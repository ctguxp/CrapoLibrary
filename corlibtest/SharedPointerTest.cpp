#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;
using namespace Global;

namespace corlibtest
{
	TEST_CLASS(SharedPointerTest)
	{
	public:
		
		TEST_METHOD(ConstructorOne)
		{
    SharedPtr<int> p1(new int(8));
    auto i = p1.Get();
    SharedPtr<int> p2(p1);
    auto j = p2.Get();
    Assert::AreEqual<int*>(i, j, L"#01");
    SharedPtr<int> p3(new int(4));
    p3 = p2;
		}

    TEST_METHOD(ConstructorTwo)
    { 
    SharedPtr<Int32> p1(new Int32(89));
    SharedPtr<Object> p2;
    p2 = p1;
    }
	};
}