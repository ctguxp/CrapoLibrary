#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Group;
      class GroupCollection : public Collections::ICollection
        {
        private:
          Array<Group*> _list;
          int _gap;
        public:
          GroupCollection(int n, int gap);
          virtual ~GroupCollection();
          void SetValue(Group* g, int i);
          virtual int32 Count() override;
          bool IsReadOnly () { return true; }
          virtual bool IsSynchronized() override;
          virtual Collections::IEnumerator* GetEnumerator() override;
        };
      }
    }
  }
