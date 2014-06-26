#pragma once
#include "System.Text.RegularExpressions.Syntax.Group.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        class CapturingGroup : public Group, public IComparable
          {
          private:
            int32    _gid;
		        GCString _name;
          public:
            CapturingGroup();
            ~CapturingGroup();
            int32 Index();
            void Index(int32 /*value*/);
            String* Name();
            void Name(String* /*value*/);
            bool IsNamed();
            virtual bool IsComplex() override;
            virtual void Compile(ICompiler* cmp, bool reverse) override;
            virtual int CompareTo(Object& other) override;
          };
        }
      }
    }
  }
