#include "pch.h"
#include "System.TimeSpan.h"
#include "System.Int64.h"
#include "System.Exception.h"

namespace System
  {

  GCTimeSpan TimeSpan::MaxValue(new TimeSpan(Int64::MaxValue), true);
  GCTimeSpan TimeSpan::MinValue(new TimeSpan(Int64::MinValue), true);

  TimeSpan::TimeSpan(int64 ticks)
    :_ticks(ticks)
    {
    }

  TimeSpan::TimeSpan(int hours, int minutes, int seconds)
    :_ticks(0)
    {
    CalculateTicks(0, hours, minutes, seconds, 0, true);
    }

  TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
    {
    CalculateTicks(days, hours, minutes, seconds, milliseconds, true);
    }

  TimeSpan::TimeSpan(const TimeSpan& ts)
    :_ticks(ts._ticks)
    {
    }

  TimeSpan::~TimeSpan()
    {
    }

  TimeSpan& TimeSpan::operator =(const TimeSpan& ts)
    {
    if(this == &ts)
      return *this;

    _ticks = ts._ticks;
    return *this;
    }

  int32 TimeSpan::Days()
    {
    return (int32)(_ticks / 864000000000L);
    }

  int32 TimeSpan::Hours()
    {
    return (int32)(_ticks % 864000000000L / 36000000000L);
    }

  int32 TimeSpan::Milliseconds()
    {
    return (int32)(_ticks % 10000000L / 10000L);
    }

  int32 TimeSpan::Minutes()
    {
    return (int32)(_ticks % 36000000000L / 600000000L);
    }

  int32 TimeSpan::Seconds()
    {
    return (int32)(_ticks % 600000000L / 10000000L);
    }

  int64 TimeSpan::Ticks()
    {
    return _ticks;
    }

  double TimeSpan::TotalDays()
    {
    return (double)_ticks / 864000000000.0;
    }

  double TimeSpan::TotalHours()
    {
    return (double)_ticks / 36000000000.0;
    }

  double TimeSpan::TotalMilliseconds()
    {
    return (double)_ticks / 10000.0;
    }

  double TimeSpan::TotalMinutes()
    {
    return (double)_ticks / 600000000.0;
    }

  double TimeSpan::TotalSeconds()
    {
    return (double)_ticks / 10000000.0;
    }

  bool TimeSpan::CalculateTicks(int days, int hours, int minutes, int seconds, int milliseconds, bool throwExc)
    {
    // there's no overflow checks for hours, minutes, ...
    // so big hours/minutes values can overflow at some point and change expected values
    int hrssec = (hours * 3600); // break point at (Int32.MaxValue - 596523)
    int minsec = (minutes * 60);
    int64 t = ((int64)(hrssec + minsec + seconds) * 1000L + (int64)milliseconds);
    t *= 10000;

    bool overflow = false;
    // days is problematic because it can overflow but that overflow can be 
    // "legal" (i.e. temporary) (e.g. if other parameters are negative) or 
    // illegal (e.g. sign change).
    if(days > 0) 
      {
      int64 td = TicksPerDay * days;
      if (t < 0) 
        {
        int64 ticks = t;
        t += td;
        // positive days -> total ticks should be lower
        overflow = (ticks > t);
        }
      else
        {
        t += td;
        // positive + positive != negative result
        overflow = (t < 0);
        }
      }
    else if(days < 0) 
      {
      int64 td = TicksPerDay * days;
      if (t <= 0) {
        t += td;
        // negative + negative != positive result
        overflow = (t > 0);
        }
      else 
        {
        int64 ticks = t;
        t += td;
        // negative days -> total ticks should be lower
        overflow = (t > ticks);
        }
      }

    if(overflow) 
      {
      if(throwExc)
        throw ArgumentOutOfRangeException(L"The timespan is too big or too small.");
      return false;
      }

    _ticks = t;
    return true;
    }

  }

