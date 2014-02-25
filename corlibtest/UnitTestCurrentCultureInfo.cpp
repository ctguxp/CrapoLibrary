#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace CrapoCoreLibrary
{
	TEST_CLASS(UnitTestCurrentCultureInfo)
	{
	public:
		
		TEST_METHOD(CurrentThread)
		{
			try
      {
      Assert::AreEqual<int>(1033, Threading::Thread::CurrentThread().CurrentCulture().LCID());
      Assert::AreEqual(L"en-US", (cstring)Threading::Thread::CurrentThread().CurrentCulture().Name());
      }
      catch(Exception& ex)
      {
      Assert::Fail(ex.Message());
      }
		}

	};
}