#include "pch.h"
#include "System.Text.RegularExpressions.CaptureCollection.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      CaptureCollection::CaptureCollection(sizet n)
        :_list(n)
        {
        }
      CaptureCollection::~CaptureCollection()
        {
        }
      int32 CaptureCollection::Count()
        {
        return (int32)_list.Length();
        }
      bool CaptureCollection::IsSynchronized()
        {
        return false;
        }
      Collections::IEnumerator* CaptureCollection::GetEnumerator()
		    {
			    return nullptr;
		    }
      void CaptureCollection::SetValue(Capture* cap, int32 i)
		    {
        _list[i] = cap;
		    }
      }
    }
  }
