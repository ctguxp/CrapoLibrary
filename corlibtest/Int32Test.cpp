#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(Int32Test)
    {
    private:
      Int32 _myInt32_1;
      Int32 _myInt32_2;
      Int32 _myInt32_3;

    public:
      Int32Test()
        :_myInt32_1(-42)
        ,_myInt32_2((-2147483647 - 1))
        ,_myInt32_3(2147483647)
        {
        }

      TEST_METHOD(TestMinMax)
        {
        Assert::AreEqual<int32>(Int32::MinValue, _myInt32_2, L"#A01");
        Assert::AreEqual<int32>(Int32::MaxValue, _myInt32_3, L"#A02");
        }

      TEST_METHOD(TestCompareTo)
        {
        GCObject val1(new Int32(-42));
        Assert::IsTrue(_myInt32_3.CompareTo(_myInt32_2) > 0, L"_myInt32_3.CompareTo(MyInt32_2) > 0");
        Assert::IsTrue(_myInt32_2.CompareTo(_myInt32_2) == 0, L"_myInt32_2.CompareTo(MyInt32_2) == 0");
        Assert::IsTrue(_myInt32_1.CompareTo((*val1)) == 0, L"_myInt32_1.CompareTo((Int32)(-42)) == 0");
        Assert::IsTrue(_myInt32_2.CompareTo(_myInt32_3) < 0, L"_myInt32_2.CompareTo(MyInt32_3) < 0");
        try 
          {
          GCObject val2(new Int16(100));
          _myInt32_2.CompareTo((*val2));
          Assert::Fail(L"Should raise a System.ArgumentException");
          }
        catch(Exception& ex) 
          {
          ArgumentException test;
          Assert::IsTrue(Object::IsInstance(test, ex), L"ex is an ArgumentException");
          }
        }

      TEST_METHOD(TestEquals)
        {
        GCObject val1(new Int32(-42));
        GCObject val2(new SByte(-42));
        Assert::IsTrue(_myInt32_1.Equals(&_myInt32_1), L"#B01");
        Assert::IsTrue(_myInt32_1.Equals(val1.Get()), L"#B02");
        Assert::IsTrue(_myInt32_1.Equals(val2.Get()) == false, L"#B03");
        Assert::IsTrue(_myInt32_1.Equals(&_myInt32_2) == false, L"#B04");
        }

      TEST_METHOD(TestGetHashCode)
        {
        try 
          {
          _myInt32_1.GetHashCode();
          _myInt32_2.GetHashCode();
          _myInt32_3.GetHashCode();
          }
        catch(Exception) 
          {
          Assert::Fail(L"GetHashCode should not raise an exception here");
          }
        }

      TEST_METHOD(TestCustomToString)
        {
        Int32 i = 123;

        Assert::AreEqual(L"00123", i.ToString(L"00000"), L"Custom format string 00000");
        Assert::AreEqual(L"123", i.ToString(L"####"), L"Custom format string ####");
        Assert::AreEqual(L"0123", i.ToString(L"0###"), L"Custom format string ####");
        Assert::AreEqual(L"0123", i.ToString(L"#0###"), L"Custom format string ####");
        Assert::AreEqual(L"000123", i.ToString(L"0#0###"), L"Custom format string ####");
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
          Assert::AreEqual(L"¤437.00", myInt.ToString(L"C", &cultInt));

          Globalization::CultureInfo cultIntE(2057); // English - United Kingdom
          Assert::AreEqual(L"£437.00", myInt.ToString(L"C", &cultIntE));

          Globalization::CultureInfo cultIntES("en-GB");
          Assert::AreEqual(L"£437.00", myInt.ToString(L"C", &cultIntES));

          myInt = 17843;
          Assert::AreEqual(L"17843.00", myInt.ToString(L"F", &cultInt));

          myInt = -29541;
          Assert::AreEqual(L"-29541.000", myInt.ToString(L"F3", &cultInt));

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