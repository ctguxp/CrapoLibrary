#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
{
	TEST_CLASS(MemoryStreamTest)
	{
	public:
		
		TEST_METHOD(ConstructorsOne)
		{
		using namespace IO;
    MemoryStream ms;
    Assert::AreEqual<uintptr>(0, ms.Length(), L"#01");
    Assert::AreEqual<uintptr>(0, ms.Capacity(), L"#02");
    Assert::AreEqual<bool>(true, ms.CanWrite(), L"#03");
		}

    TEST_METHOD(ConstructorsTwo)
    {
    using namespace IO;
    MemoryStream ms(10);
    Assert::AreEqual<uintptr>(0, ms.Length(), L"#01");
    Assert::AreEqual<uintptr>(10, ms.Capacity(), L"#02");
    ms.Capacity(0);
    ByteArray buffer = ms.GetBuffer();
    Assert::AreEqual<int>(-1, ms.ReadByte(), L"#03");
    Assert::IsTrue(buffer.IsNull(), L"#04");
    ByteArray newBuffer(5);
    ms.Read(newBuffer, 0, (int)newBuffer.Length());
    Assert::AreEqual<uintptr>(0, ms.Position(), L"#05");
    Assert::AreEqual<uintptr>(0, ms.Length(), L"#06");
    }

	};
}