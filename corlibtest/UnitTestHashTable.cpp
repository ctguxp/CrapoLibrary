#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(UnitTestHashTable)
    {
    BEGIN_TEST_CLASS_ATTRIBUTE()
      TEST_CLASS_ATTRIBUTE(L"Namespace", L"System.Collections")
      END_TEST_CLASS_ATTRIBUTE()

    public:
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
            Object* obj = h1.Get(&key);
            Char* val = reinterpret_cast<Char*>(obj);
            Assert::AreEqual<wchar_t>(values[i], (*val));
            }
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