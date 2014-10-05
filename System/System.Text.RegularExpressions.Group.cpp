#include "pch.h"
#include "System.Text.RegularExpressions.Group.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {

      AutoPtr<Group> Group::_fail = new Group();

      Group::Group()
        :Capture()
        ,_success(false)
        ,_captures(0)
        {
        }
      Group::Group(String text, int index, int length)
        :Capture(text, index, length)
        ,_success(true)
        ,_captures(0)
        {
        }
      Group::Group(String text, int index, int length, int n_caps)
        :Capture(text, index, length)
        ,_success(true)
			  ,_captures(n_caps)
        {
			  _captures.SetValue(this, n_caps - 1);
		    }
      Group::~Group()
        {
        }
      bool Group::Success()
        {
        return _success;
        }
      Group* Group::Fail()
        {
        return _fail.Get();
		    }
      }
    }
  }
