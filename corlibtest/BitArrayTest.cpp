#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(BitArrayTest)
    {
    private:
      Collections::BitArray _testBa;
      BoolArray             _testPattern;

      void VerifyPattern(Collections::BitArray& ba, BoolArray& pattern)
        {
        Assert::AreEqual<int32>(ba.Length(), (int32)pattern.Length());
        for(sizet i = 0; i < pattern.Length(); i++)
          Assert::AreEqual(ba.Get(i), pattern[i]);
        }
    public:
      BitArrayTest()
        :_testBa(70)
        ,_testPattern(70)
        {
        int32 i;
        for(i = 0; i < (int32)_testPattern.Length() / 2; i++)
          _testPattern[i] = ((i % 2) == 0);
        for(; i < (int32)_testPattern.Length(); i++)
          _testPattern[i] = ((i % 2) != 0);

        for(i = 0; i < _testBa.Length()/2; i++)
          _testBa.Set(i, ((i % 2) == 0));
        for(; i < _testBa.Length(); i++)
          _testBa.Set(i, ((i % 2) != 0));
        }		

      TEST_METHOD(TestBoolConstructor)
        {
        using namespace Collections;
        BitArray ba(_testPattern);
        VerifyPattern(ba, _testPattern);
        }

      TEST_METHOD(TestCopyConstructor) 
        {
        using namespace Collections;
        BitArray ba(_testBa);
        VerifyPattern(ba, _testPattern);
        }

      TEST_METHOD(TestByteConstructor)
        {
        using namespace Collections;
        byte bytes[] = { 0xaa, 0x55, 0xaa, 0x55, 0x80 };
        ByteArray byteArr(bytes, 5);
        BitArray ba(byteArr);

        Assert::AreEqual<int32>(ba.Length(), (int32)byteArr.Length() * 8, L"Lengths not equal");

        //// spot check
        Assert::IsTrue(ba.Get(7), L"7 not true");
        Assert::IsTrue(!ba.Get(6), L"6 not false");
        Assert::IsTrue(!ba.Get(15), L"15 not false");
        Assert::IsTrue(ba.Get(14), L"14 not true");
        Assert::IsTrue(ba.Get(39), L"39 not true");
        Assert::IsTrue(!ba.Get(35), L"35 not false");
        }

      TEST_METHOD(TestIntConstructor)
        {
        using namespace Collections;
        int ints[] = { ~0x55555555, 0x55555551 };
        IntArray intArr(ints, 2); 
        BitArray ba(intArr);

        Assert::AreEqual<int32>(ba.Length(), (int32)intArr.Length() * 32);

        //// spot check
        Assert::IsTrue(ba.Get(31));
        Assert::IsTrue(!ba.Get(30));
        Assert::IsTrue(!ba.Get(63));
        Assert::IsTrue(ba.Get(62));
        Assert::IsTrue(ba.Get(32));
        Assert::IsTrue(!ba.Get(35));
        }

      TEST_METHOD(TestClone)
        {
        using namespace Collections;
        BitArray ba;
        ba = _testBa;

        VerifyPattern(ba, _testPattern);

        // ensure that changes in ba don't get propagated to testBa
        ba.Set(0, false);
        ba.Set(1, false);
        ba.Set(2, false);

        VerifyPattern(_testBa, _testPattern);
        }

      TEST_METHOD(TestSetLength)
        {
        int32 origLen = _testBa.Length();
        _testBa.Length(_testBa.Length() + 33);

        Assert::AreEqual<int32>(origLen + 33, _testBa.Length());
        for(int i = origLen; i < _testBa.Length(); i++)
          _testBa.Set(i,true);

        _testBa.Length(_testBa.Length() - 33);
        VerifyPattern(_testBa, _testPattern);
        }
    };
  }