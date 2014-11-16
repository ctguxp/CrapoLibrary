#include "pch.h"
#include "System.Text.RegularExpressions.Interval.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      Interval::Interval(int32 low, int32 high)
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

      Interval::Interval(const Interval& inter)
        :_low(inter._low)
        ,_high(inter._high)
        ,_contiguous(inter._contiguous)
        {
        }

      Interval::~Interval()
        {
        }

      Interval& Interval::operator=(const Interval& inter)
        {
        if(this == &inter)
          return *this;

        _low = inter._low;
        _high = inter._high;
        _contiguous = inter._contiguous;
        return *this;
        }

      bool Interval::IsDiscontiguous() 
        {
        return !_contiguous;
        }

      bool Interval::IsSingleton()
        { 
        return _contiguous && _low == _high;
        }

      bool Interval::IsRange()
        { 
        return !IsSingleton() && !IsEmpty();
        }

      bool Interval::IsDisjoint(Interval* i) 
        {
        if(IsEmpty() || i->IsEmpty())
          return true;

        return !(_low <= i->High() && i->Low() <= _high);
        }

      bool Interval::IsAdjacent(Interval* i)
        {
        if(IsEmpty() || i->IsEmpty())
          return false;

        return _low == i->High() + 1 || _high == i->Low() - 1;
        }

      int32 Interval::Size() 
        {
        if(IsEmpty())
          return 0;

        return _high - _low + 1;
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

      void Interval::Merge(Interval* i)
        {
        if(i->IsEmpty())
          return;
        if(IsEmpty())
          {
          _low = i->Low();
          _high = i->High();
          }

        if(i->Low() < _low)
          _low = i->Low();
        if(i->High() > _high)
          _high = i->High();
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

      int32 IntervalCollection::Count() 
        {
        return _intervals.Count(); 
        }

      bool IntervalCollection::IsSynchronized()
        {
        return false;
        }

      Collections::IEnumerator* IntervalCollection::GetEnumerator()
        {
        throw NotImplementedException();
        }

      Interval* IntervalCollection::Get(int32 index)
        {
        return (Interval*)_intervals[index].Get();
        }

      void IntervalCollection::Sort () 
        {
        _intervals.Sort();
        }

      void IntervalCollection::Normalize () 
        {
        _intervals.Sort();

        int j = 0;
        while (j < _intervals.Count() - 1) 
          {
          Interval* a = (Interval*)_intervals[j].Get();
          Interval* b = (Interval*)_intervals[j + 1].Get();

          if (!a->IsDisjoint(b) || a->IsAdjacent(b)) 
            {
            a->Merge(b);
            //_intervals[j].Reset(a);
            _intervals.RemoveAt(j + 1);
            }
          else
            ++j;
          }
        }

      IntervalCollection* IntervalCollection::GetMetaCollection(CostDelegate cost_del) 
        {
        IntervalCollection* meta = new IntervalCollection();

        Normalize();
        Optimize(0, Count() - 1, meta, cost_del);
        meta->_intervals.Sort();

        return meta;
        }

      void IntervalCollection::Optimize(int32 begin, int32 end, IntervalCollection* meta, CostDelegate cost_del)
        {
        Interval set;
        set.Contiguous(false);

        int32 best_set_begin = -1;
        int32 best_set_end = -1;
        double best_set_cost = 0;

        for(int32 i = begin; i <= end; ++ i) 
          {
          Interval* intervalI = (Interval*)_intervals[i].Get();
          set.Low(intervalI->Low());

          double cost = 0.0;
          for (int j = i; j <= end; ++ j) 
            {
            Interval* intervalJ = (Interval*)_intervals[j].Get();
            set.High(intervalJ->High());
            cost += cost_del(intervalJ);

            double set_cost = cost_del(&set);
            if (set_cost < cost && cost > best_set_cost) 
              {
              best_set_begin = i;
              best_set_end = j;
              best_set_cost = cost;
              }
            }
          }

        if (best_set_begin < 0)
          {
          // didn't find an optimal set: add original members

          for(int i = begin; i <= end; ++ i)
            {
            meta->Add((Interval*)_intervals[i].Get());
            }
          }
        else {
          // found set: add it ...

          Interval* intervalH = (Interval*)_intervals[best_set_begin].Get();
          set.Low(intervalH->Low());
          Interval* intervalL = (Interval*)_intervals[best_set_begin].Get();
          set.High(intervalL->High());

          meta->Add(&set);

          // ... and optimize to the left and right

          if(best_set_begin > begin)
            Optimize (begin, best_set_begin - 1, meta, cost_del);
          if (best_set_end < end)
            Optimize (best_set_end + 1, end, meta, cost_del);
          }
        }
      }
    }
  }
