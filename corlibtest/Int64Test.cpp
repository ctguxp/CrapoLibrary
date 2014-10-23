#include "pch.h"
#include "CppUnitTest.h"
#include "CustomFramework.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(Int64Test)
    {
    private:
      Int64 _myInt64_1;
      Int64 _myInt64_2;
      Int64 _myInt64_3;

    public:
    public:
      Int64Test()
        :_myInt64_1(-42)
        ,_myInt64_2((-9223372036854775807i64 - 1))
        ,_myInt64_3(9223372036854775807)
        {
        }

      TEST_METHOD(TestMinMax)
        {
        Assert::AreEqual<int64>(Int64::MinValue, _myInt64_2);
        Assert::AreEqual<int64>(Int64::MaxValue, _myInt64_3);
        }

      TEST_METHOD(TestCompareTo)
        {
        GCObject val1(new Int64(-42));
        Assert::IsTrue(_myInt64_3.CompareTo(_myInt64_2) > 0);
        Assert::IsTrue(_myInt64_2.CompareTo(_myInt64_2) == 0);
        Assert::IsTrue(_myInt64_1.CompareTo((*val1)) == 0);
        Assert::IsTrue(_myInt64_2.CompareTo(_myInt64_3) < 0);
        try 
          {
          GCObject val2(new Int16(100));
          _myInt64_2.CompareTo((*val2));
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
        GCObject val1(new Int64(-42));
        GCObject val2(new SByte(-42));
        Assert::IsTrue(_myInt64_1.Equals(&_myInt64_1));
        Assert::IsTrue(_myInt64_1.Equals(val1.Get()));
        Assert::IsTrue(_myInt64_1.Equals(val2.Get()) == false);
        Assert::IsTrue(_myInt64_1.Equals(&_myInt64_2) == false);
        }

      TEST_METHOD(TestGetHashCode)
        {
        try 
          {
          _myInt64_1.GetHashCode();
          _myInt64_2.GetHashCode();
          _myInt64_3.GetHashCode();
          }
        catch(Exception) 
          {
          Assert::Fail(L"GetHashCode should not raise an exception here");
          }
        }
    };
  }