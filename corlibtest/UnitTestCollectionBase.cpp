#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  class ConcreteCollection : public Collections::CollectionBase
    {
    public:
      ConcreteCollection(sizet i)
        {
        using namespace Collections;
        IList* listObj = this;

        for(sizet j = 0; j< i; j++)
          {
          listObj->Add(GCObject(new UInt64(j)));
          }
        }
    };

  TEST_CLASS(UnitTestCollectionBase)
    {
    public:

      TEST_METHOD(TestCount)
        {
        ConcreteCollection cc(4);
        Assert::IsTrue(4 == cc.Count());
        }

      TEST_METHOD(TestRemove)
        {
        int numberOfItems = 3;
        // Set up a test collection
        ConcreteCollection myCollection(numberOfItems);

        // The list is 0-based.  So if we remove the second one
        myCollection.RemoveAt(1);

        // We should see the original third one in it's place
        //Assert.IsTrue (myCollection.PeekAt(1) == 2);
        //Assert.IsTrue (myCollection.onRemoveFired);
        //Assert.IsTrue (myCollection.onRemoveIndex == 1);
        //Assert.IsTrue (myCollection.onRemoveCompleteFired);
        //Assert.IsTrue (myCollection.onRemoveCompleteIndex == 1);
        //IList listObj = myCollection;
        //listObj.Remove(0);
        //// Confirm parameters are being passed to the hooks
        //Assert.IsTrue (myCollection.onRemoveIndex == 0);
        //Assert.IsTrue (myCollection.onRemoveCompleteIndex == 0);
        }
    };
  }