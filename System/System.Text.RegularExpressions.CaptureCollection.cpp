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
      sizet CaptureCollection::Count()
        {
        return _list.Length();
        }
      bool CaptureCollection::IsSynchronized()
        {
        return false;
        }
      Collections::IEnumerator* CaptureCollection::GetEnumerator()
		    {
			    return nullptr;
		    }
      void CaptureCollection::SetValue(Capture* cap, sizet i)
		    {
        _list[i] = cap;
		    }
      }
    }
  }
