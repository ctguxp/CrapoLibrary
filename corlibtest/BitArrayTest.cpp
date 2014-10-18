#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  static int op1[] = { 0x33333333, 0x33333333 };
  static int op2[] = { 0x66666666, 0x66666666 };

  static IntArray p1(op1, 2);
  static IntArray p2(op2, 2);

  TEST_CLASS(BitArrayTest)
    {
    private:
      Collections::BitArray _testBa;
      BoolArray             _testPattern;
      Collections::BitArray _op1;
      Collections::BitArray _op2;

      void VerifyPattern(Collections::BitArray& ba, BoolArray& pattern)
        {
        Assert::AreEqual<int32>(ba.Length(), (int32)pattern.Length());
        for(sizet i = 0; i < pattern.Length(); i++)
          Assert::AreEqual(ba.Get((int32)i), pattern[i]);
        }
    public:
      BitArrayTest()
        :_testBa(70)
        ,_testPattern(70)
        ,_op1(p1)
        ,_op2(p2)
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
        Assert::IsTrue(ba[7], L"7 not true");
        Assert::IsTrue(!ba[6], L"6 not false");
        Assert::IsTrue(!ba[15], L"15 not false");
        Assert::IsTrue(ba[14], L"14 not true");
        Assert::IsTrue(ba[39], L"39 not true");
        Assert::IsTrue(!ba[35], L"35 not false");
        }

      TEST_METHOD(TestIntConstructor)
        {
        using namespace Collections;
        int ints[] = { ~0x55555555, 0x55555551 };
        IntArray intArr(ints, 2); 
        BitArray ba(intArr);

        Assert::AreEqual<int32>(ba.Length(), (int32)intArr.Length() * 32);

        //// spot check
        Assert::IsTrue(ba[31]);
        Assert::IsTrue(!ba[30]);
        Assert::IsTrue(!ba[63]);
        Assert::IsTrue(ba[62]);
        Assert::IsTrue(ba[32]);
        Assert::IsTrue(!ba[35]);
        }

      TEST_METHOD(TestValConstructor)
        {
        using namespace Collections;
        BitArray ba_false(64, false);

        Assert::AreEqual<int32>(ba_false.Length(), 64);
        int32 i = 0;
        for(; i < ba_false.Length(); ++i)
          Assert::IsTrue(!ba_false[i]);

        BitArray ba_true(64, true);

        Assert::AreEqual<int32>(ba_true.Length(), 64);
        for(i = 0; i < ba_true.Length(); ++i)
          Assert::IsTrue(ba_true[i]);
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

      TEST_METHOD(TestCopyToBool)
        {
        try 
          {
          BoolArray barray(_testBa.Length() + 10);

          _testBa.CopyTo<bool>(barray, 5);

          for(int32 i = 0; i < (int32)_testBa.Length(); i++)
            Assert::AreEqual<bool>(_testBa[i], barray[i+5]);
          }
        catch(Exception& ex)
          {
          String msg(L"Unexpected exception thrown: ");
          msg += ex.Message();
          Assert::Fail((cstring)msg);
          }
        }

      TEST_METHOD(TestCopyToInt)
        {
        try 
          {
          _testBa.Length(34);
          IntArray iarray(2 + 10);

          _testBa.CopyTo<int32>(iarray, 5);

          Assert::AreEqual<int32>(0x55555555, iarray[5]);
          // FIXME:  Same thing here as in TestCopyToByte
          //Assert::AreEqual<int32>(0x01, iarray[6]);
          }
        catch(Exception& ex)
          {
          String msg(L"Unexpected exception thrown: ");
          msg += ex.Message();
          Assert::Fail((cstring)msg);
          }
        }

      TEST_METHOD(TestCopyToByte)
        {
        try 
          {
          _testBa.Length(34);
          ByteArray barray(5 + 10);

          _testBa.CopyTo<byte>(barray, 5);

          for(int32 i = 5; i < 9; i++)
            Assert::AreEqual<int32>(0x55, barray[i] & 0xff);

          // FIXME: MS fails on the next line.  This is because
          // we truncated testBa.Length, and MS's internal array still
          // has the old bits set.  CopyTo() doesn't say specifically
          // whether the "junk" bits (bits past Length, but within Length
          // rounded up to 32) will be copied as 0, or if those bits are
          // undefined.
          //Assert::AreEqual<int32>(0x01, barray[9] & 0xff);
          }
        catch(Exception& ex)
          {
          String msg(L"Unexpected exception thrown: ");
          msg += ex.Message();
          Assert::Fail((cstring)msg);
          }
        }

      TEST_METHOD(CopyToEmptyEmpty) 
        {
        using namespace Collections;
        BitArray bitarray(0);

        IntArray intarray(1);

        bitarray.CopyTo<int32>(intarray, 0);
        }

      TEST_METHOD(TestAnd)
        {
        using namespace Collections;
        BitArray result = _op1.And(_op2);
        Assert::AreEqual<int32>(result.Length(), _op1.Length());
        for(int32 i = 0; i < result.Length(); )
          {
          Assert::IsTrue(!result[i++]);
          Assert::IsTrue(result[i++]);
          Assert::IsTrue(!result[i++]);
          Assert::IsTrue(!result[i++]);
          }
        }

      TEST_METHOD(TestOr)
        {
        using namespace Collections;
        BitArray result = _op1.Or(_op2);
        Assert::AreEqual<int32>(result.Length(), _op1.Length());
        for(int32 i = 0; i < result.Length(); )
          {
          Assert::IsTrue(result[i++]);
          Assert::IsTrue(result[i++]);
          Assert::IsTrue(result[i++]);
          Assert::IsTrue(!result[i++]);
          }
        }

      TEST_METHOD(TestNot)
        {
        using namespace Collections;
        BitArray result = _op1.Not();
        Assert::AreEqual<int32>(result.Length(), _op1.Length());
        for(int32 i = 0; i < result.Length(); )
          {
          Assert::IsTrue(!result[i++]);
          Assert::IsTrue(!result[i++]);
          Assert::IsTrue(result[i++]);
          Assert::IsTrue(result[i++]);
          }
        }

      TEST_METHOD(TestXor)
        {
        using namespace Collections;
        BitArray result = _op1.Xor(_op2);
        Assert::AreEqual<int32>(result.Length(), _op1.Length());
        for(int32 i = 0; i < result.Length(); )
          {
          Assert::IsTrue(result[i++]);
          Assert::IsTrue(!result[i++]);
          Assert::IsTrue(result[i++]);
          Assert::IsTrue(!result[i++]);
          }
        }

      TEST_METHOD(TestSetAll)
        {
        _testBa.SetAll(false);
        int32 i = 0;
        for(; i < _testBa.Length(); ++i)
          Assert::IsTrue(!_testBa[i]);
        _testBa.SetAll(true);
        for(i = 0; i < _testBa.Length(); ++i)
          Assert::IsTrue(_testBa[i]);
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

      TEST_METHOD(TestEnumerator)
        {
        using namespace Collections;
        try 
          {
          GCIEnumerator e(_testBa.GetEnumerator());

          int32 i = 0;
          for(; e->MoveNext(); i++)
            {
            Boolean& cur = (Boolean&)(*e->Current().Get());
            Assert::AreEqual<bool>(cur, _testPattern[i]);
            }

          Assert::IsTrue(!e->MoveNext());
          // read, to make sure reading isn't considered a write.
          bool b = _testBa[0];

          e->Reset();
          for(i = 0; e->MoveNext(); i++)
            {
            Boolean& cur = (Boolean&)(*e->Current().Get());
            Assert::AreEqual<bool>(cur, _testPattern[i]);
            }

          try
            {
            e->Reset();
            _testBa.Set(0, !_testBa[0]);
            e->MoveNext();
            Assert::Fail(L"IEnumerator.MoveNext() should throw when collection modified.");
            }
          catch(SystemException&)
          //catch(InvalidOperationException ex)
            {
            }
          }
        catch(Exception& ex)
          {
          String msg(L"Unexpected exception thrown: ");
          msg += ex.Message();
          Assert::Fail((cstring)msg);
          }
        }
    };
  }