#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(ArrayTest)
    {
    public:

      TEST_METHOD(TestSort)
        {
        wchar_t starter[] = {L'd', L'b', L'f', L'e', L'a', L'c'};
        String shit[] = {L"Roxy", L"Mark", L"Chris", L"Caleb"};
        CharArray c1(starter, 6);
        StringArray c2(shit, 4);
        Array<wchar_t>::Sort(c1, 0, (int32)c1.Length());
        Array<String>::Sort(c2, 0, (int32)c2.Length());
        Assert::AreEqual<wchar_t>(L'a', c1[0], L"#N21");
        Assert::AreEqual<wchar_t>(L'b', c1[1], L"#N22");
        Assert::AreEqual<wchar_t>(L'c', c1[2], L"#N23");
        Assert::AreEqual<wchar_t>(L'd', c1[3], L"#N24");
        Assert::AreEqual<wchar_t>(L'e', c1[4], L"#N25");
        Assert::AreEqual<wchar_t>(L'f', c1[5], L"#N26");

        Assert::AreEqual(L"Caleb", c2[0], L"#N27");      
        Assert::AreEqual(L"Chris", c2[1], L"#N27");
        Assert::AreEqual(L"Mark", c2[2], L"#N27");
        Assert::AreEqual(L"Roxy", c2[3], L"#N28");
        }

    };
  }