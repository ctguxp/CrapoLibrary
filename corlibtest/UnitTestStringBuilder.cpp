#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace CrapoCoreLibrary
  {
  TEST_CLASS(UnitTestStringBuilder)
    {
    public:

      TEST_METHOD(TestConstructor1)
        {
        using namespace Text;
        StringBuilder sb;
        Assert::AreEqual((cstring)String::Empty(), (cstring)sb.ToString());
        Assert::AreEqual<sizet>(0, sb.Length());
        Assert::AreEqual<sizet>(16, sb.Capacity());
        }

      TEST_METHOD(TestConstructor2)
        {
        using namespace Text;

        // check ctor that specifies the capacity
        StringBuilder sb1(10);
        Assert::AreEqual((cstring)String::Empty(), (cstring)sb1.ToString());
        Assert::AreEqual<sizet>(0, sb1.Length());
        // check that capacity is not less than default
        Assert::AreEqual<sizet>(10, sb1.Capacity());

        // TODO : Copy constuctor for StringBuilder would be nice
        StringBuilder sb2(42);
        Assert::AreEqual((cstring)String::Empty(), (cstring)sb2.ToString());
        Assert::AreEqual<sizet>(0, sb2.Length());
        // check that capacity is set
        Assert::AreEqual<sizet>(42, sb2.Capacity());
        }

      TEST_METHOD(TestConstructor3)
        {
        using namespace Text;
        // check ctor that specifies the capacity & maxCapacity
        StringBuilder sb(444, 1234);
        Assert::AreEqual((cstring)String::Empty(), (cstring)sb.ToString());
        Assert::AreEqual<sizet>(0, sb.Length());
        Assert::AreEqual<sizet>(444, sb.Capacity());
        Assert::AreEqual<sizet>(1234, sb.MaxCapacity());
        }

      TEST_METHOD(TestConstructor4)
        {
        using namespace Text;
        // check for exception in ctor that specifies the capacity & maxCapacity
        try 
          {
          StringBuilder sb(9999, 15);
          }
        catch(ArgumentOutOfRangeException) 
          {
          return;
          }
        // if we didn't catch an exception, then we have a problem Houston.
        Assert::Fail(L"Capacity exceeds MaxCapacity");
        }

      TEST_METHOD(TestConstructor8)
        {
        using namespace Text;
        // check for exception in ctor that specifies the capacity & maxCapacity
        try 
          {
          String someString(L"someString");
			    StringBuilder sb(someString, 10000, 4, 18);
          }
        catch(ArgumentOutOfRangeException) 
          {
          return;
          }
        // if we didn't catch an exception, then we have a problem Houston.
        Assert::Fail(L"StartIndex and length must refer to a location within the string.");
        }

    TEST_METHOD(TestConstructor9)
        {
        using namespace Text;
        // check for exception in ctor that specifies the capacity & maxCapacity
        try 
          {
          String someString(L"someString");
			    StringBuilder sb(someString, 4, 4000, 18);
          }
        catch(ArgumentOutOfRangeException) 
          {
          return;
          }
        // if we didn't catch an exception, then we have a problem Houston.
        Assert::Fail(L"StartIndex and length must refer to a location within the string.");
        }

    TEST_METHOD(TestConstructor10)
        {
        using namespace Text;
        String someString(L"someString");
        StringBuilder sb(someString, 4, 6, 18);
		    String expected(L"String");
		    Assert::AreEqual((cstring)expected, (cstring)sb.ToString());
        }

      TEST_METHOD(TestAppend)
        {
        using namespace Text;
        StringBuilder sb(L"Foo");
        sb.Append(L"Two");
        String expected(L"FooTwo");
        Assert::AreEqual((cstring)expected, (cstring)sb.ToString());
        }

    };
  }