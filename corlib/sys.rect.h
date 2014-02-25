// Perch Lake Computer System

#pragma once

namespace sys
  {
  class rect
    {
    public:
     // Default constructor
      rect(long left = 0, long top = 0, long right = 0, long bottom = 0);
      // Destructor
      ~rect();
      // Get Area property
      long get_area() { return get_height() * get_width(); }
      // Get Bottom property
      long get_bottom() { return _bottom; }
      // Get Height property
      long get_height() { return _bottom - _top; }
      // Get Left property
      long get_left() { return _left; }
      // Get Right property
      long get_right() { return _right; }
      // Get Top property
      long get_top() { return _top; }
      // Get Width property
      long get_width() { return _right - _left; }
    private:
       long _left;    // The x-coordinate of the upper-left corner of the rectangle
       long _top;     // The y-coordinate of the upper-left corner of the rectangle.
       long _right;   // The x-coordinate of the lower-right corner of the rectangle
       long _bottom;  // The y-coordinate of the lower-right corner of the rectangle
    };
  }
