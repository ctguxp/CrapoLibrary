#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace CrapoCoreLibrary
{
	TEST_CLASS(UnitTestASCII)
	{
	public:
		
		TEST_METHOD(ByteCount)
		{
    Text::ASCIIEncoding enc;
    String duffy(L"Duffy is my dog");
    int n = enc.GetByteCount(duffy);
    Assert::AreEqual<int>(15, n);
		}

    TEST_METHOD(GetBytesTelegu)
		{
    try
      {
      Text::ASCIIEncoding enc;
      String duffy(L"అధ్యక్షుడు హియర్");
      ByteArray n = enc.GetBytes(duffy);
      Assert::AreEqual<sizet>(16, n.Length());
      for(uint32 i = 0; i < n.Length(); ++i)
        {
        if(i != 10)
          Assert::AreEqual<byte>(0x3F, n[i]);
        else
          Assert::AreEqual<byte>(0x20, n[i]);
        }
      int c = enc.GetCharCount(n, 0, (int)n.Length());
      Assert::AreEqual<int>(16, c);
      }
    catch(Exception& ex)
      {
      Assert::Fail(ex.Message());
      }
    catch(std::exception ex)
      {
      Assert::Fail(L"Standard Exception Thrown");
      }
		}

	};
}