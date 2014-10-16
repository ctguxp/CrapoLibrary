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
            Match* m = regex.Match(L"the TEST", 0);
            if(m == nullptr)
              {
              throw SystemException(L"null ptr");
              }
            if(m->Success())
              {
              Assert::AreEqual(L"T", (cstring)m->Value());
              }
            delete m;
            }
          catch(Exception& ex)
            {
            Assert::Fail(ex.Message());
            }
          }
        }
    };
  }
