#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      class Interval;

      typedef double (*CostDelegate)(Interval* i);

      class Interval : public Object, public IComparable
        {
        private:
          int  _low;
          int  _high;
          bool _contiguous;
        public:
          Interval(int32 low = 0, int32 high = 0);
          Interval(const Interval& inter);
          virtual ~Interval();
          Interval& operator=(const Interval& inter);
          bool IsDiscontiguous();
          bool IsSingleton();
          bool IsRange();
          bool IsDisjoint(Interval* i);
          bool IsAdjacent(Interval* i);
          void Merge(Interval* i);
          int32 Size();
          virtual int CompareTo(Object&) override;
          bool Contains(Interval& i);
          bool Contains(int i) { return _low <= i && i <= _high; }
          bool Contiguous() { return _contiguous; }
          void Contiguous(bool value) { _contiguous = value; }
          int Low() { return _low; }
          void Low(int value) { _low = value; }
          int High() { return _high; }
          void High(int value) { _high = value; }
          bool IsEmpty() { return _low > _high; }
          bool Intersects(Interval& i);
        };

      class IntervalCollection : public Collections::ICollection
        {
        private:
          Collections::ArrayList _intervals;
        public:
          IntervalCollection();
          virtual ~IntervalCollection();
          void Add(Interval*);
          Interval* Get(int32 index);
          void Sort();
          void Normalize();
          virtual int32 Count() override;
          virtual bool IsSynchronized() override;
          virtual Collections::IEnumerator* GetEnumerator() override;
          IntervalCollection* GetMetaCollection(CostDelegate cost_del);
        private:
          void Optimize(int32 begin, int32 end, IntervalCollection* meta, CostDelegate cost_del);
        };
      }
    }
  }
