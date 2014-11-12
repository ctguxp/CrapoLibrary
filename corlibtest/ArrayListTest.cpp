#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(ArrayListTest)
    {
    public:

      TEST_METHOD(TestCtor)
        {
        using namespace Collections;
          {
          GCArrayList al1(new ArrayList());
          Assert::IsNotNull(al1.Get(), L"no basic ArrayList");
          }
          {
          bool errorThrown = false;
          try 
            {
            GCArrayList a(new ArrayList(nullptr));
            } 
          catch(ArgumentNullException&) 
            {
            errorThrown = true;
            }
          Assert::IsTrue(errorThrown, L"null icollection error not thrown");
          }
          //{
          //// what can I say?  I like chars.  [--DB]
          //char [] coll = { 'a', 'b', 'c', 'd' };
          //ArrayList al1 = new ArrayList (coll);
          //Assert.IsNotNull (al1, "no icollection ArrayList");
          //for (int i = 0; i < coll.Length; i++)
          //  {
          //  Assert.AreEqual (coll [i], al1 [i], i + " not ctor'ed properly.");
          //  }
          //}
          //{
          //try 
          //  {
          //  Char [,] c1 = new Char [2, 2];
          //  ArrayList al1 = new ArrayList (c1);
          //  Assert.Fail ("Should fail with multi-dimensional array in constructor.");
          //  } 
          //catch (RankException) 
          //  {
          //  }
          //}

          {
          bool errorThrown = false;
          try 
            {
            GCArrayList a(new ArrayList(-1));
            } 
          catch(ArgumentOutOfRangeException&)
            {
            errorThrown = true;
            }
          Assert::IsTrue(errorThrown, L"negative capacity error not thrown");
          }
        }

    };
  }