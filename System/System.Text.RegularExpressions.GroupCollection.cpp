#include "pch.h"
#include "System.Text.RegularExpressions.GroupCollection.h"
#include "System.Text.RegularExpressions.Group.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      GroupCollection::GroupCollection(int n, int gap)
        :_list(n)
        ,_gap(gap)
        {
        }
      GroupCollection::~GroupCollection()
        {
        }
      void GroupCollection::SetValue(Group* g, int i)
		    {
			  _list[i] = g;
		    }
      int32 GroupCollection::Count() 
        {
			  return (int32)_list.Length();
		    }
      bool GroupCollection::IsSynchronized()
        { 
        return false;
        }
      Collections::IEnumerator* GroupCollection::GetEnumerator()
        {
        return nullptr;
        }
      }
    }
  }
