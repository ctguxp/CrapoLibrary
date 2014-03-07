#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
{
	TEST_CLASS(UnitTestDouble)
	{
	public:
		
		TEST_METHOD(Exponent)
		{
    Double value(12345.6789);
    Globalization::CultureInfo cultInt;
    Assert::AreEqual(L"1.2345678900E+004", value.ToString(L"E10", &cultInt));
		}

	};
}