#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.ExpressionCollection.h"

using namespace Global;

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        ExpressionCollection::ExpressionCollection()
          {
          }
        ExpressionCollection::~ExpressionCollection()
          {
          }
        void ExpressionCollection::Add(SharedPtr<Expression>& e)
          {
          GCObject temp = e;
          InnerList().Add(temp);
          }
        sizet ExpressionCollection::Add(GCObject& value)
          {
          return CollectionBase::Add(value);
          }
        }
      }
    }
  }
