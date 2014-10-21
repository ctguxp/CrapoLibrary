#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {

  class MyTextWriter : public IO::TextWriter
    {
    public:
      MyTextWriter()
        :IO::TextWriter(&Globalization::CultureInfo::InvariantCulture())
        {
        }
      void UpdateLine()
        {
        _coreNewLine.Length(1);
        _coreNewLine[0] = L'Z';
        }
       void UpdateLine2()
        {
        _coreNewLine[0] = L'Y';
        }
    };

  TEST_CLASS(TextWriterTest)
    {
    public:
      TEST_METHOD(CoreNewLine)
        {
        MyTextWriter w;
        Assert::IsTrue(w.NewLine().Length() != 0, L"#01");

        w.UpdateLine();
        Assert::AreEqual<wchar_t>(L'Z', w.NewLine()[0], L"#02");

        w.UpdateLine2();
        Assert::AreEqual<wchar_t>(L'Y', w.NewLine()[0], L"#03");
        }
    };
  }