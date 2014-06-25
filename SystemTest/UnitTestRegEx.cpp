#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace SystemTest
  {		
  TEST_CLASS(UnitTestRegEx)
    {
    public:

      TEST_METHOD(TestConstructor)
        {
        using namespace Text;
          {
          try
            {
            using namespace RegularExpressions;
            Regex regex(L"[A-Z]", RegexOptions::None);
            }
          catch(Exception& ex)
            {
            Assert::Fail(ex.Message());
            }
          }
        }
    };
  }
