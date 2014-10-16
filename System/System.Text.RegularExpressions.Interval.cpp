#include "pch.h"
#include "System.Text.RegularExpressions.Interval.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      Interval::Interval(int low, int high)
        :_low(low)
        ,_high(high)
        ,_contiguous(true)
        {
        if(_low > _high)
          {
          int t = _low;
          _low = _high;
          _high = t;
          }
        }
      Interval::~Interval()
        {
        }
      int Interval::CompareTo(Object& obj) 
        {
        Interval& i = dynamic_cast<Interval&>(obj);
        return _low - i._low;
        }

      bool Interval::Contains(Interval& i) 
        {
        if(!IsEmpty() && i.IsEmpty())
          return true;
        if(IsEmpty())
          return false;

        return _low <= i._low && i._high <= _high;
        }

      bool Interval::Intersects(Interval& i)
        {
        if(IsEmpty() || i.IsEmpty())
          return false;

        return ((Contains(i._low) && !Contains(i._high)) || (Contains(i._high) && !Contains(i._low)));
        }	


      IntervalCollection::IntervalCollection()
        {
        }

      IntervalCollection::~IntervalCollection()
        {
        }

      void IntervalCollection::Add(Interval* i) 
        {
        GCObject t(i);
        _intervals.Add(t);
        }

      void IntervalCollection::Normalize () 
        {
        /*_intervals.Sort();

        int j = 0;
        while (j < _intervals.Count() - 1) 
          {
          Interval a = (Interval)_intervals[j];
          Interval b = (Interval)_intervals[j + 1];

          if (!a.IsDisjoint(b) || a.IsAdjacent(b)) 
            {
            a.Merge(b);
            _intervals[j] = a;
            _intervals.RemoveAt(j + 1);
            }
          else
            ++ j;
          }*/
        }

      IntervalCollection* IntervalCollection::GetMetaCollection (CostDelegate /*cost_del*/) 
        {
        IntervalCollection* meta = new IntervalCollection();

        /*Normalize();
        Optimize(0, Count - 1, meta, cost_del);
        meta->intervals.Sort();*/

        return meta;
        }
      }
    }
  }
