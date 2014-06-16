#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.ExpressionCollection.h"

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
        void ExpressionCollection::Add(Expression* e)
          {
          InnerList().Add(e);
          }
        sizet ExpressionCollection::Add(Object* value)
          {
          return CollectionBase::Add(value);
          }
        }
      }
    }
  }
