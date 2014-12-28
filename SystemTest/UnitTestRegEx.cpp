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
            //String pattern(L"\\d+");
            String pattern(L"[A-Z]+");
            Regex regex(pattern, RegexOptions::None);
            //GCMatch m = regex.Match(L"Dot 55 Perls", 0);
            GCMatch m = regex.Match(L"this is a TEST of the", 0);
            if(m.Get() == nullptr)
              {
              throw SystemException(L"null ptr");
              }
            if(m->Success())
              {
              //Assert::AreEqual(L"55", (cstring)m->Value());
              Assert::AreEqual(L"TEST", (cstring)m->Value());
              }
            else
              {
              Assert::Fail(L"Success Failed");
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
