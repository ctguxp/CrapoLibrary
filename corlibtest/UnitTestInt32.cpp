#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
{
	TEST_CLASS(UnitTestInt32)
	{
	public:
		TEST_CLASS_INITIALIZE(Startup)
    {
    }

		TEST_METHOD(Simple)
		{
    try
      {
		  Int32 myInt(-42);
      Assert::AreEqual(L"-42", myInt.ToString());
      String f1(L"G");
      Assert::AreEqual(L"-42", myInt.ToString(f1));

      myInt = 437;
      f1 = L"D";
      Assert::AreEqual(L"437", myInt.ToString(f1));

      f1 = L"C";
      Assert::AreEqual(L"$437.00", myInt.ToString(f1));

      Globalization::CultureInfo cultInt;  // Invariant Culture
      Assert::AreEqual(L"�437.00", myInt.ToString(L"C", &cultInt));

      Globalization::CultureInfo cultIntE(2057); // English - United Kingdom
      Assert::AreEqual(L"�437.00", myInt.ToString(L"C", &cultIntE));

      Globalization::CultureInfo cultIntES("en-GB");
      Assert::AreEqual(L"�437.00", myInt.ToString(L"C", &cultIntES));

      myInt = 123456789;
      f1 = L"N1";
      Assert::AreEqual(L"123,456,789.0", myInt.ToString(f1));

      myInt = 96;
      f1 = L"P2";
      Assert::AreEqual(L"9,600.00 %", myInt.ToString(f1));

      myInt = Int32::MinValue;
      Assert::AreEqual(L"-2147483648", myInt.ToString());
      myInt = Int32::MaxValue;
      Assert::AreEqual(L"2147483647", myInt.ToString());
      myInt = 16;
      f1 = L"X";
      Assert::AreEqual(L"10", myInt.ToString(f1));
      }
    catch(Exception& ex)
      {
      Assert::Fail(ex.Message());
      }
		}
	};
}