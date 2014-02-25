#include "pch.h"
#include "System.Single.h"
#include <cfloat>

#define NAN ((double)0.0/(double)DBL_MIN)

namespace System
{
  float Single::NaN = NAN;

  Single::Single()
    :m_value()
    {
    }
  Single::~Single()
    {
    }
}
