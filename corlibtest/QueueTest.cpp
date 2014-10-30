#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(QueueTest)
    {
    private:
      Collections::Queue _q1;
      Collections::Queue _q2;
      Collections::Queue _emptyQueue;
    public:

      QueueTest()
        :_q1(10)
        ,_q2(50, 1.5f)
        ,_emptyQueue()
        {
        int32 i = 0;
        for(;i < 100; ++i)
          {
          GCObject x(new Int32(i));
          _q1.Enqueue(x);
          }

        for(i = 50; i < 100; ++i)
          {
          GCObject x(new Int32(i));
          _q2.Enqueue(x);
          }
        }

      TEST_METHOD(TestConstructorException1) 
        {
        using namespace Collections;
        try 
          {
          Queue q(-1, 2);
          Assert::Fail(L"Should throw an exception");
          } 
        catch(ArgumentOutOfRangeException& ex)
          {
          Assert::AreEqual(L"capacity", ex.ParamName(), L"Exception's ParamName must be \"capacity\"");
          }
        }

      TEST_METHOD(TestConstructorException2) 
        {
        using namespace Collections;
        try 
          {
          Queue q(10, 0);
          Assert::Fail(L"Should throw an exception because growFactor < 1");
          } 
        catch (ArgumentOutOfRangeException& ex) 
          {
          Assert::AreEqual(L"growFactor", ex.ParamName(), L"Exception's ParamName must be \"growFactor\"");
          }
        }

      TEST_METHOD(TestConstructorException3) 
        {
        using namespace Collections;
        try 
          {
          Queue q(10, 11);
          Assert::Fail(L"Should throw an exception because growFactor > 10");
          } 
        catch(ArgumentOutOfRangeException& ex) 
          {
          Assert::AreEqual(L"growFactor", ex.ParamName(), L"Exception's ParamName must be \"growFactor\"");
          }
        }

      TEST_METHOD(TestConstructors)
        {
        Assert::IsTrue(_q1.Count() == 100);
        Assert::IsTrue(_q2.Count() == 50);
        Assert::IsTrue(_emptyQueue.Count() == 0);
        }

      TEST_METHOD(TestCount) 
        {
        Assert::AreEqual<int32>(100, _q1.Count(), L"Count #1");
        int32 i = 1;
        for(; i <= 50; ++i) 
          {
          _q1.Dequeue();
          }
        Assert::AreEqual<int32>(50, _q1.Count(), L"Count #2");
        for(i = 1; i <= 50; ++i) 
          {
          GCObject x(new Int32(i));
          _q1.Enqueue(x);
          }
        Assert::AreEqual<int32>(100, _q1.Count(), L"Count #3");
        Assert::AreEqual<int32>(50, _q2.Count(), L"Count #4");
        Assert::AreEqual<int32>(0, _emptyQueue.Count(), L"Count #5");
        }

      TEST_METHOD(TestClone) 
        {
        using namespace Collections;
        Queue q3(_q2);
        Assert::IsTrue(q3.Count() == _q2.Count());
        for(int32 i = 0; i < 50; i++)
          {
          GCObject two = _q2.Dequeue();
          GCObject three = q3.Dequeue();
          Assert::IsTrue(two->Equals(three.Get()));
          }
        Assert::IsTrue(q3.Count() == 0);
        Assert::IsTrue(_q2.Count() == 0);
        }

      TEST_METHOD(TestContains) 
        {
        for(int32 i = 0; i < 100; ++i) 
          {
          Int32 obj(i);
          Assert::IsTrue(_q1.Contains(&obj));
          Assert::IsTrue(!_emptyQueue.Contains(&obj));
          if(i < 50)
            Assert::IsTrue(!_q2.Contains(&obj));
          else
            Assert::IsTrue(_q2.Contains(&obj));
          }

        Assert::IsTrue(!_q1.Contains(nullptr), L"q1 does not contain null");
        GCObject n;
        _q1.Enqueue(n);
        Assert::IsTrue(_q1.Contains(nullptr), L"q1 contains null");
        }

      TEST_METHOD(TestClear) 
        {
        _q1.Clear();
        Assert::IsTrue(_q1.Count() == 0);
        _q2.Clear();
        Assert::IsTrue(_q2.Count() == 0);
        _emptyQueue.Clear();
        Assert::IsTrue(_emptyQueue.Count() == 0);
        }

      TEST_METHOD(TestDequeue) 
        {
        using namespace Collections;
        Queue queue;
        StringArray tmp(50);
        int32 i = 0;
        for(;i < 50; ++i)
          {
          Int32 x(i);
          tmp[i] = String::Format(L"Data # {0}", &x);
          GCObject y(new String(tmp[i]));
          queue.Enqueue(y);
          }

        i = 0;
        while(queue.Count() > 0)
          {
          GCObject q = queue.Dequeue();
          String& z = static_cast<String&>((*q));
          Assert::AreEqual((cstring)tmp[i], (cstring)(z), (cstring)tmp[i]);
          i++;
          }
        }
    };
  }