#pragma once

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      enum class Position
        {
        Any,			// anywhere
        Start,			// start of string			\A
        StartOfString,		// start of string			\A
        StartOfLine,		// start of line			^
        StartOfScan,		// start of scan			\G
        End,			// end or before newline at end		\Z
        EndOfString,		// end of string			\z
        EndOfLine,		// end of line				$
        Boundary,		// word boundary			\b
        NonBoundary		// not word boundary			\B
        };
      }
    }
  }