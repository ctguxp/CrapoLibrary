#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(HashTableTest)
    {
    BEGIN_TEST_CLASS_ATTRIBUTE()
      TEST_CLASS_ATTRIBUTE(L"Namespace", L"System.Collections")
      END_TEST_CLASS_ATTRIBUTE()

    public:

      TEST_METHOD(TestCtor1) 
        {
        using namespace Collections;
        GCHashtable h(new Hashtable());
        Assert::IsNotNull(h.Get(), L"No hash table");
        }

      TEST_METHOD(TestCtor2)
        {
        using namespace Collections;
          {
          bool errorThrown = false;
          try
            {
            Hashtable h((IDictionary*)nullptr);
            }
          catch(ArgumentException)
            {
            errorThrown = true;
            }
          Assert::IsTrue(errorThrown, L"null hashtable error not thrown");
          }
          {
          String keys[4] = { L"this", L"is", L"a", L"test" };
          Char values[4] = { L'a', L'b', L'c', L'd' };
          Hashtable h1;
          int i  = 0;
          for(; i < 4; ++i)
            {
            h1.Add(new String(keys[i]), new Char(values[i]));
            }

          for(i = 0; i < 4; ++i)
            {
            String key(keys[i]);
            Char* val = reinterpret_cast<Char*>(h1.Get(&key));
            Assert::AreEqual<wchar_t>(values[i], (*val));
            }
          }
        }

      TEST_METHOD(TestCtor3)
        {
        using namespace Collections;
        try
          {
          GCHashtable h(new Hashtable());
          GCHashtable hh(new Hashtable(h.Get(), Single::NaN));
          Assert::Fail(L"Should have throw ArgumentOutOfRangeException");
          }
        catch(Exception& ex)
          {
          ArgumentOutOfRangeException test;
          if(!Object::IsInstance(test, ex))
            Assert::Fail(L"Exception type should be ArgumentOutOfRangeException");
          }
        }

      TEST_METHOD(TestCtor4)
        {
        using namespace Collections;
        try
          {
          GCHashtable ht(new Hashtable(Int32::MaxValue, 0.1f, nullptr, nullptr));
          Assert::Fail(L"Should have throw ArgumentOutOfRangeException");
          }
        catch(Exception& ex)
          {
          ArgumentException test;
          if(!Object::IsInstance(test, ex))
            Assert::Fail(L"Exception type should be ArgumentException");
          }
        }

      TEST_METHOD(TestIsFixedSize)
        {
        using namespace Collections;
        GCHashtable h(new Hashtable());
        Assert::AreEqual(false, h->IsFixedSize(), L"hashtable not fixed by default");
        // TODO - any way to get a fixed-size hashtable?
        }

      TEST_METHOD(TestCount) 
        {
        using namespace Collections;
        Hashtable h;
        Assert::AreEqual<int32>(0, h.Count(), L"new table - count zero");
        int32 max = 100;
        for(int32 i = 1; i <= max; i++)
          {
          Int32* p = new Int32(i);
          h.Add(p, new Int32(i));
          String c = String::Format(L"Count wrong for {0}", p);
          Assert::AreEqual<int32>(i, h.Count(), c);
          }
        for(int32 i = 1; i <= max; i++)
          {
          Int32 p(i);
          h.Set(new Int32(i), new Int32(i * 2));
          String c = String::Format(L"Count shouldn't change at {0}", &p);
          Assert::AreEqual<int32>(max, h.Count(), c);
          }
        }

      TEST_METHOD(TestClear)
        {
        using namespace Collections;
        Hashtable h;
        Assert::AreEqual<sizet>(0, h.Count(), L"new table - count zero");
        int max = 100;
        for(int i = 1; i < max; i++)
          h.Add(new Int32(i), new Int32(i));
        Assert::IsTrue(h.Count() > 0, L"table - don't gots stuff");
        h.Clear();
        Assert::AreEqual<sizet>(0, h.Count(), L"Table should be cleared");
        }

      TEST_METHOD(TestAdd) 
        {
        using namespace Collections;
          {
          bool errorThrown = false;
          try 
            {
            Hashtable h;
            h.Add(nullptr, new String(L"huh?"));
            } 
          catch(ArgumentNullException& ex) 
            {
            errorThrown = true;
            Assert::AreEqual(L"key", ex.ParamName(), L"ParamName is not 'key'");
            }
          Assert::IsTrue(errorThrown, L"null add error not thrown");
          }
          {
          bool errorThrown = false;
          try 
            {
            Hashtable h;
            h.Add(new Char(L'a'), new Int32(1));
            h.Add(new Char(L'a'), new Int32(2));
            } 
          catch(ArgumentException&) 
            {
            errorThrown = true;
            }
          Assert::IsTrue(errorThrown, L"re-add error not thrown");
          }
          // TODO - hit NotSupportedException
          {
          Hashtable h;
          int32 max = 100;
          for(int32 i = 1; i <= max; i++)
            {
            h.Add(new Int32(i), new Int32(i));
            Int32 t(i);  
            Int32& x = static_cast<Int32&>((*h.Get(&t)));
            Assert::AreEqual<int32>(i, x, L"value wrong for i");
            }
          }
        }

      TEST_METHOD(TestContains)
        {
        using namespace Collections;
          {
          bool errorThrown = false;
          try 
            {
            Hashtable h;
            bool result = h.Contains(nullptr);
            } 
          catch(ArgumentNullException& ex) 
            {
            errorThrown = true;
            Assert::AreEqual(L"key", ex.ParamName(), L"ParamName is not 'key'");
            }
          Assert::IsTrue(errorThrown, L"null add error not thrown");
          }
          {
          Hashtable h = new Hashtable();
          for(int32 i = 0; i < 10000; i += 2) 
            {
            h.Add(new Int32(i), new Int32(i));
            }
          for(int32 i = 0; i < 10000; i += 2) 
            {
            Int32 first(i);
            Int32 second(i+1);
            String fStr(L"hashtable must contain ");
            fStr += first.ToString();
            Assert::IsTrue(h.Contains(&first), fStr);
            Assert::IsTrue (!h.Contains(&second), L"hashtable does not contain ");/*+((int)(i+1)).ToString()*/
            }
          }
        }

      TEST_METHOD(TestKeys) 
        {
        using namespace Collections;
        String keys[4] = {"this", "is", "a", "test"};
        String keys2[2] = {"new", "keys"};
        Char values1[4] = {'a', 'b', 'c', 'd'};
        Char values2[4] = {'e', 'f', 'g', 'h'};
        ICollection* keysReference = nullptr;
        ICollection* keysReference2 = nullptr;
        Hashtable h1;
        for (int32 i = 0; i < 4; i++) 
          {
          h1.Add(new String(keys[i]), new Char(values2[i]));
          }
        Assert::AreEqual<int32>(4, h1.Keys()->Count(), L"keys wrong size");
        for (int32 i = 0; i < 4; i++)
          {
          h1.Set(new String(keys[i]), new Char(values2[i]));
          }
        Assert::AreEqual<int32>(4, h1.Keys()->Count(), L"keys wrong size 2");

        // MS .NET Always returns the same reference when calling Keys property
        keysReference = h1.Keys();
        keysReference2 = h1.Keys();
        Assert::IsTrue(keysReference == keysReference2, L"keys references differ");

        for(int32 i = 0; i < 2; i++) 
          {
          h1.Add(new String(keys2[i]), new Char(values2[i]));
          }
        Assert::AreEqual<int32>(6, h1.Keys()->Count(), L"keys wrong size 3");
        Assert::AreEqual<int32>(6, keysReference->Count(), L"keys wrong size 4");
        }

      TEST_METHOD(TestContainsValue) 
        {
        using namespace Collections;
          {
          Hashtable h;
          String test(L"blue");
          h.Add(new Char(L'a'), new String(L"blue"));
          Assert::IsTrue(h.ContainsValue(&test), L"blue? it's in there!");
          test = L"green";
          Assert::IsTrue(!h.ContainsValue(&test), L"green? no way!");
          Assert::IsTrue(!h.ContainsValue(nullptr), L"null? no way!");
          h.Add(new Char(L'b'), nullptr);
          Assert::IsTrue(h.ContainsValue(nullptr), L"null? it's in there!");
          }
        }
    };
  }