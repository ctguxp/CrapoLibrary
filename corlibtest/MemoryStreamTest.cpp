#include "pch.h"
#include "CppUnitTest.h"
#include "CustomFramework.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;
using namespace Global;

namespace corlibtest
  {
  TEST_CLASS(MemoryStreamTest)
    {
    private:
      SharedPtr<ByteArray> _testStreamData;
      SharedPtr<IO::MemoryStream> _testStream;

    public:
      MemoryStreamTest()
        :_testStreamData(new ByteArray(100))
        {

        for(sizet i = 0; i < _testStreamData->Length(); i++)
          (*_testStreamData)[i] = (byte)(100 - i);

        _testStream.Reset(new IO::MemoryStream(_testStreamData));
        }

      void VerifyTestData(String id, ByteArray& testBytes, int32 start, int32 count)
        {
        if(testBytes.IsNull())
          Assert::Fail(id + L"+1 testBytes is null");

        if(start < 0 || count < 0  ||
          start + count > (int32)_testStreamData->Length() ||
          start > (int32)_testStreamData->Length())
          throw ArgumentOutOfRangeException (id + L"+2");

        for(int test = 0; test < count; test++) 
          {
          if(testBytes[test] == (*_testStreamData)[start + test])
            continue;

          String failStr = L"testByte {0} (testStream {1}) was <{2}>, expecting <{3}>";
          ObjectArray objArr(4);
          objArr.Add(0, *new Int32(test));
          objArr.Add(1, *new Int32(start + test));
          objArr.Add(2, *new Byte(testBytes[test]));
          objArr.Add(3, *new Byte((*_testStreamData)[start + test]));
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
        Assert::AreEqual<int64>(0, ms.Position(), L"#05");
        Assert::AreEqual<int64>(0, ms.Length(), L"#06");
        }

      TEST_METHOD(ConstructorsThree)
        {
        using namespace IO;
        MemoryStream ms(_testStreamData);
        Assert::AreEqual<int64>(100, ms.Length(), L"#01");
        Assert::AreEqual<int64>(0, ms.Position(), L"#02");
        }

      TEST_METHOD(ConstructorFour)
        {
        using namespace IO;
        MemoryStream ms(_testStreamData, true);
        Assert::AreEqual<int64>(100, ms.Length(), L"#01");
        Assert::AreEqual<int64>(0, ms.Position(), L"#02");
        ms.Position(50);
        byte saved = (*_testStreamData)[50];
        try
          {
          ms.WriteByte(23);
          Assert::AreEqual<byte>(23, (*_testStreamData)[50], L"#03");
          }
        catch(Exception& /*ex*/)
          {
          }
        (*_testStreamData)[50] = saved;
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
        MemoryStream ms(_testStreamData, 50, 50);
        Assert::AreEqual<int64>(50, ms.Length(), L"#01");
        Assert::AreEqual<int64>(0, ms.Position(), L"#02");
        Assert::AreEqual<int32>(50, ms.Capacity(), L"#03");
        ms.Position(1);
        byte saved = (*_testStreamData)[51];
        try
          {
          ms.WriteByte(23);
          Assert::AreEqual<byte>(23, (*_testStreamData)[51], L"#04");
          } 
        catch(Exception& /*ex*/)
          {
          }
        (*_testStreamData)[50] = saved;
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
        _testStream->Read(readBytes, 0, 10);
        VerifyTestData(L"R1", readBytes, 0, 10);

        ///* Seek back to beginning */

        _testStream->Seek(0, SeekOrigin::Begin);

        ///* Read again, bit more this time */
        _testStream->Read(readBytes, 0, 20);
        VerifyTestData(L"R2", readBytes, 0, 20);

        ///* Seek to 20 bytes from End */
        _testStream->Seek(-20, SeekOrigin::End);
        _testStream->Read(readBytes, 0, 20);
        VerifyTestData(L"R3", readBytes, 80, 20);

        int32 readByte = _testStream->ReadByte();
        Assert::AreEqual<int32>(-1, readByte, L"R4");
        }

      TEST_METHOD(GetBufferOne)
        {
        using namespace IO;
        MemoryStream ms;
        ByteArray buffer = ms.GetBuffer();
        Assert::AreEqual<int64>(0, buffer.Length(), L"#01");
        }

      TEST_METHOD(GetBufferTwo)
        {
        using namespace IO;
        MemoryStream ms(100);
        ByteArray buffer = ms.GetBuffer();  
        Assert::AreEqual<int64>(100, buffer.Length(), L"#01");

        ms.Write((*_testStreamData), 0, 100);
        ms.Write((*_testStreamData), 0, 100);
        Assert::AreEqual<int64>(200, ms.Length(), L"#02");
        buffer = ms.GetBuffer ();
        Assert::AreEqual<int64>(256, buffer.Length(), L"#03");
        }

      TEST_METHOD(WriteBytes)
        {
        using namespace IO;
        ByteArray readBytes(100);

        MemoryStream ms(100);

        for(int i = 0; i < 100; i++)
          ms.WriteByte((*_testStreamData)[i]);

        ms.Seek(0, SeekOrigin::Begin); 
        ms.Read(readBytes, 0, 100);
        VerifyTestData(L"W1", readBytes, 0, 100);
        }

      TEST_METHOD(WriteByte)
        {
        using namespace IO;
        MemoryStream ms(100);
        ms.Write((*_testStreamData), 0, 100);
        ms.Position(100);
        ms.WriteByte(101);
        Assert::AreEqual<int64>(101, ms.Position(), L"#01");
        Assert::AreEqual<int64>(101, ms.Length(), L"#02");
        Assert::AreEqual<int32>(256, ms.Capacity(), L"#03");
        ms.Write((*_testStreamData), 0, 100);
        ms.Write((*_testStreamData), 0, 100);
        // 301
        Assert::AreEqual<int64>(301, ms.Position(), L"#04");
        Assert::AreEqual<int64>(301, ms.Length(), L"#05");
        Assert::AreEqual<int32>(512, ms.Capacity(), L"#06");
        }

      TEST_METHOD(WriteBlock)
        {
        using namespace IO;
        ByteArray readBytes(100);

        MemoryStream ms(100);

        ms.Write((*_testStreamData), 0, 100);
        ms.Seek(0, SeekOrigin::Begin); 
        _testStream->Read(readBytes, 0, 100);
        VerifyTestData(L"WB1", readBytes, 0, 100);
        ByteArray arrayBytes = _testStream->ToArray();
        Assert::AreEqual<int>(100, (int)arrayBytes.Length(), L"#01");
        VerifyTestData(L"WB2", arrayBytes, 0, 100);
        }

      TEST_METHOD(PositionLength)
        {
        using namespace IO;
        MemoryStream ms;
        ms.Position(4);
        ms.WriteByte((byte)'M');
        ms.WriteByte((byte)'O');
        Assert::AreEqual<int64>(6, ms.Length(), L"#01");
        Assert::AreEqual<int64>(6, ms.Position(), L"#02");
        ms.Position(0);
        Assert::AreEqual<int64>(0, ms.Position(), L"#03");
        }

      TEST_METHOD(MorePositionLength)
        {
        try
          {
          using namespace IO;
          MemoryStream ms(_testStreamData);
          ms.Position(101);
          Assert::AreEqual<int64>(101, ms.Position(), L"#01");
          Assert::AreEqual<int64>(100, ms.Length(), L"#02");
          ms.WriteByte(1); // This should throw the exception
          Assert::Fail(L"NotSupportedException not thrown");
          }
        catch(NotSupportedException&)
          {
          }

        }

    }; // End of Class
  }