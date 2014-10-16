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
          Interval(int low, int high);
          virtual ~Interval();
          virtual int CompareTo(Object&) override;
          bool Contains(Interval& i);
          bool Contains(int i) { return _low <= i && i <= _high; }
          int Low() { return _low; }
          void Low(int value) { _low = value; }
          int High() { return _high; }
          void High(int value) { _high = value; }
          bool IsEmpty() { return _low > _high; }
          bool Intersects(Interval& i);
        };

      class IntervalCollection
        {
        private:
          Collections::ArrayList _intervals;
        public:
          IntervalCollection();
          ~IntervalCollection();
          void Add(Interval*);
          void Normalize();
          IntervalCollection* GetMetaCollection(CostDelegate cost_del);
        };
      }
    }
  }
