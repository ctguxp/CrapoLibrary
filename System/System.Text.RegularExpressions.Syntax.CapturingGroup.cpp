#include "pch.h"
#include "System.Text.RegularExpressions.Syntax.CapturingGroup.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      namespace Syntax
        {
        CapturingGroup::CapturingGroup()
          :_gid(0)
          ,_name()
          {
          }
        CapturingGroup::~CapturingGroup()
          {
          }
        int32 CapturingGroup::Index()
          { 
          return _gid; 
          }
        String* CapturingGroup::Name()
          { 
          return _name.Get();
          }
        void CapturingGroup::Name(String* value)
          {
          _name.Reset(value);
          }
        void CapturingGroup::Index(int32 value)
          { 
          _gid = value;
          }
        bool CapturingGroup::IsNamed()
          { 
          return _name.Get() != nullptr;
          }
        bool CapturingGroup::IsComplex()
          {
          return true;
          }
        void CapturingGroup::Compile(ICompiler* cmp, bool reverse)
          {
          cmp->EmitOpen(_gid);
          Group::Compile(cmp, reverse);
          cmp->EmitClose(_gid);
          }
        int CapturingGroup::CompareTo(Object& other)
		      {
          CapturingGroup& cg = static_cast<CapturingGroup&>(other);
			    return _gid - cg._gid;
		      }
        }
      }
    }
  }
