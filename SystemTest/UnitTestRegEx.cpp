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
            String pattern(L"[A-Z]+");
            Regex regex(pattern, RegexOptions::None);
            GCMatch m = regex.Match(L"the TEST", 0);
            if(m.Get() == nullptr)
              {
              throw SystemException(L"null ptr");
              }
            if(m->Success())
              {
              Assert::AreEqual(L"TEST", (cstring)m->Value());
              }
            }
          catch(Exception& ex)
            {
            Assert::Fail(ex.Message());
            }
          }
        }

      TEST_METHOD(Simple)
        {
        using namespace Text;
        wchar_t c1[] = { (wchar_t)32, (wchar_t)8212, (wchar_t)32 };
        CharArray c(c1, 3);
        String s(c);
         Assert::IsTrue(RegularExpressions::Regex::IsMatch(s, s), L"char");
        }
    };
  }
