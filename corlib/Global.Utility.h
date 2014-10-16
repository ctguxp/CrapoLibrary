#pragma once

namespace Global
  {
  template<class T>
  inline void Swap(T& left, T& right)
    {
    T tmp = left;
    left = right;
    right = tmp;
    }
  }
