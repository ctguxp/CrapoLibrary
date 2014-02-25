#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
{
	TEST_CLASS(UnitTestUTF8cpp)
	{
	public:
		
		TEST_METHOD(ByteCount)
		{
    Text::UTF8Encoding enc;
    String duffy(L"Duffy is my dog");
    int n = enc.GetByteCount(duffy);
    Assert::AreEqual<int>(15, n);
		}

    TEST_METHOD(GetBytesTelegu)
		{
    try
      {
      Text::UTF8Encoding enc;
      String duffy(L"అధ్యక్షుడు హియర్");
      ByteArray n = enc.GetBytes(duffy);
      Assert::AreEqual<sizet>(46, n.Length());
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