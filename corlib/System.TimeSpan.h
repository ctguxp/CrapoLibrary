#pragma once
#include "System.Object.h"
#include "AutoPtr.h"

namespace System
  {
  class TimeSpan;
  typedef AutoPtr<TimeSpan> GCTimeSpan;
  class CRAPOCOREDLL_API TimeSpan : public Object
    {
    private:
      enum class ParseError
        {
        None,
        Format,
        Overflow
        };
      enum class FormatElementType
        {
        Days,
        Hours,
        Minutes,
        Seconds,
        Ticks,
        TicksUppercase,
        Literal,
        EscapedChar,
        Error,
        End
        };
    public:
      enum : int64
        {
        TicksPerDay = 864000000000L,
        TicksPerHour = 36000000000L,
        TicksPerMillisecond = 10000L,
        TicksPerMinute = 600000000L,
        TicksPerSecond = 10000000L
        };
    private:
      int64 _ticks;
    public:
#pragma warning(disable:4251)
      static GCTimeSpan MaxValue;
      static GCTimeSpan MinValue;
#pragma warning(default:4251)
    public:
      TimeSpan(int64 ticks = 0);
      TimeSpan(const TimeSpan&);
      TimeSpan(int hours, int minutes, int seconds);
      TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);
      ~TimeSpan();
      TimeSpan& operator =(const TimeSpan&);
      int32 Days();
      int32 Hours();
      int32 Milliseconds();
      int32 Minutes();
      int32 Seconds();
      int64 Ticks();
      double TotalDays();
      double TotalHours();
      double TotalMilliseconds();
      double TotalMinutes();
      double TotalSeconds();
    private:
      bool CalculateTicks(int days, int hours, int minutes, int seconds, int milliseconds, bool throwExc);
    };
  }

