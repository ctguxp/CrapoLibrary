#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;
using namespace Global;

namespace corlibtest
  {
  TEST_CLASS(MemoryStreamTest)
    {
    private:
      SharedPtr<ByteArray> testStreamData;

    public:
      MemoryStreamTest()
        :testStreamData(new ByteArray(100))
        {

        for(sizet i = 0; i < testStreamData->Length(); i++)
          (*testStreamData.Get())[i] = (byte)(100 - i);
        }

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

      TEST_METHOD(ConstructorsThree)
        {
        using namespace IO;
        MemoryStream ms(testStreamData);
        Assert::AreEqual<uintptr>(100, ms.Length(), L"#01");
        Assert::AreEqual<uintptr>(0, ms.Position(), L"#02");
        }

      TEST_METHOD(ConstructorFour)
        {
        using namespace IO;
        MemoryStream ms(testStreamData, true);
        Assert::AreEqual<uintptr>(100, ms.Length(), L"#01");
        Assert::AreEqual<uintptr>(0, ms.Position(), L"#02");
        ms.Position(50);
        byte saved = (*testStreamData)[50];
        try
          {
          ms.WriteByte(23);
          Assert::AreEqual<byte>(23, (*testStreamData)[50], L"#03");
          }
        catch(Exception& /*ex*/)
          {
          }
        (*testStreamData)[50] = saved;
        ms.Position(100);
        try
          {
          ms.WriteByte(23);
          }
        catch (Exception& /*ex*/)
          {
          return;
          }
        Assert::Fail(L"#04");
        }

    };
  }