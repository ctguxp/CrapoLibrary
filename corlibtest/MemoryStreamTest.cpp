#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;
using namespace Global;

namespace Microsoft
  { 
  namespace VisualStudio
    { 
    namespace CppUnitTestFramework
      {
      template<> static std::wstring ToString<int64>(const int64& t)
        {
        Int64 i(t); 
        return std::wstring((cstring)i.ToString());
        }
      }
    }
  }

namespace corlibtest
  {
  TEST_CLASS(MemoryStreamTest)
    {
    private:
      SharedPtr<ByteArray> testStreamData;
      SharedPtr<IO::MemoryStream> testStream;

    public:
      MemoryStreamTest()
        :testStreamData(new ByteArray(100))
        {

        for(sizet i = 0; i < testStreamData->Length(); i++)
          (*testStreamData)[i] = (byte)(100 - i);

        testStream.Reset(new IO::MemoryStream(testStreamData));
        }


      void VerifyTestData(String id, ByteArray& testBytes, int start, int count)
        {
        if(testBytes.IsNull())
          Assert::Fail(id + L"+1 testBytes is null");

        if(start < 0 || count < 0  ||
          start + count > testStreamData->Length() ||
          start > testStreamData->Length())
          throw ArgumentOutOfRangeException (id + L"+2");

        for(int test = 0; test < count; test++) 
          {
          if(testBytes[test] == (*testStreamData)[start + test])
            continue;

          String failStr = L"testByte {0} (testStream {1}) was <{2}>, expecting <{3}>";
          ObjectArray objArr(4);
          objArr.Add(0, *new Int32(test));
          objArr.Add(1, *new Int32(start + test));
          objArr.Add(2, *new Byte(testBytes[test]));
          objArr.Add(3, *new Byte((*testStreamData)[start + test]));
          failStr = String::Format(failStr, objArr);
          Assert::Fail(id + L"-3" + failStr);
          }
        }

      TEST_METHOD(ConstructorsOne)
        {
        using namespace IO;
        MemoryStream ms;
        Assert::AreEqual<int64>(0, ms.Length(), L"#01");
        Assert::AreEqual<int32>(0, ms.Capacity(), L"#02");
        Assert::AreEqual<bool>(true, ms.CanWrite(), L"#03");
        }

      TEST_METHOD(ConstructorsTwo)
        {
        using namespace IO;
        MemoryStream ms(10);
        Assert::AreEqual<int64>(0, ms.Length(), L"#01");
        Assert::AreEqual<int32>(10, ms.Capacity(), L"#02");
        ms.Capacity(0);
        ByteArray buffer = ms.GetBuffer();
        Assert::AreEqual<int>(-1, ms.ReadByte(), L"#03");
        Assert::IsTrue(buffer.IsNull(), L"#04");
        ByteArray newBuffer(5);
        ms.Read(newBuffer, 0, (int)newBuffer.Length());
        Assert::AreEqual<uintptr>(0, ms.Position(), L"#05");
        Assert::AreEqual<int64>(0, ms.Length(), L"#06");
        }

      TEST_METHOD(ConstructorsThree)
        {
        using namespace IO;
        MemoryStream ms(testStreamData);
        Assert::AreEqual<int64>(100, ms.Length(), L"#01");
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

      TEST_METHOD(ConstructorsFive)
        {
        using namespace IO;
        MemoryStream ms(testStreamData, 50, 50);
        Assert::AreEqual<int64>(50, ms.Length(), L"#01");
        Assert::AreEqual<uintptr>(0, ms.Position(), L"#02");
        Assert::AreEqual<int32>(50, ms.Capacity(), L"#03");
        ms.Position(1);
        byte saved = (*testStreamData)[51];
        try
          {
          ms.WriteByte(23);
          Assert::AreEqual<byte>(23, (*testStreamData)[51], L"#04");
          } 
        catch(Exception& /*ex*/)
          {
          }
        (*testStreamData)[50] = saved;
        ms.Position(100);
        try
          {
          ms.WriteByte(23);
          Assert::Fail(L"#05");
          } 
        catch(NotSupportedException)
          {
          }

        try
          {
          ms.Capacity(100);
          Assert::Fail(L"#06");
          } 
        catch(NotSupportedException)
          {
          }

        try
          {
          ms.Capacity(51);
          Assert::Fail(L"#07");
          } 
        catch(NotSupportedException)
          {
          }
        Assert::AreEqual<int>(50, (int)ms.Length(), L"#08");   
        }

      TEST_METHOD(ConstructorsSix)
        {
        try
          {
          using namespace IO;
          MemoryStream ms(-2);
          Assert::Fail(L"#01");
          }
        catch(ArgumentOutOfRangeException)
          {
          }
        }

      TEST_METHOD(Read)
        {
        using namespace IO;
        ByteArray readBytes(20);

        ///* Test simple read */
        testStream->Read(readBytes, 0, 10);
        VerifyTestData(L"R1", readBytes, 0, 10);

        ///* Seek back to beginning */

        testStream->Seek(0, SeekOrigin::Begin);

        ///* Read again, bit more this time */
        testStream->Read(readBytes, 0, 20);
        VerifyTestData(L"R2", readBytes, 0, 20);

        ///* Seek to 20 bytes from End */
        testStream->Seek(-20, SeekOrigin::End);
        testStream->Read(readBytes, 0, 20);
        VerifyTestData(L"R3", readBytes, 80, 20);

        int32 readByte = testStream->ReadByte();
        Assert::AreEqual<int32>(-1, readByte, L"R4");
        }


    }; // End of Class
  }