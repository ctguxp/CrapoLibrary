#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace CrapoCoreLibrary
  {		
  TEST_CLASS(UnitTestStrings)
    {
    public:
      TEST_METHOD(DefaultStringConstructor)
        {
        using namespace System;
        String test;
        Assert::AreEqual(test, L"");
        }
      TEST_METHOD(StringConstructor)
        {
        using namespace System;
        String test(L"One 1");
        Assert::AreEqual(test, L"One 1");
        }
      TEST_METHOD(CharIndexConstructor)
        {
        using namespace System;
        String test(L"Mark W. Mohr", 8, 4);
        Assert::AreEqual(L"Mohr", test);
        }
      TEST_METHOD(ArrayConstructorNull)
        {
        using namespace System;
        CharArray empty;
        String test(empty);
        Assert::AreEqual(L"",test);
        }
      TEST_METHOD(CharOneConstructor)
        {
        using namespace System;
        String test(L'3', 4);
        Assert::AreEqual(L"3333", test);
        }
      TEST_METHOD(SByteConstructor1)
        {
        using namespace System;
        sbyte stext[] = "The rain in spain stays mostly on the plain";
        String test(stext);
        Assert::AreEqual(L"The rain in spain stays mostly on the plain", test);
        }
      TEST_METHOD(SByteConstructor2)
        {
        using namespace System;
        String test((sbyte*)nullptr);
        Assert::AreEqual(L"", test);
        }
      TEST_METHOD(SByteConstructor3)
        {
        using namespace System;
        String test((sbyte*)nullptr, 0, 0);
        Assert::AreEqual(L"", test);
        }
      TEST_METHOD(SByteConstructor4)
        {
        using namespace System;
        sbyte stext[] = "The rain in spain stays mostly on the plain";
        String test(stext, 4, 4);
        Assert::AreEqual(L"rain", test);
        }
      TEST_METHOD(ContainsTest)
        {
        using namespace System;
        String test(L"Mark Mohr Was Here");
        String last(L"Was");
        Assert::IsTrue(test.Contains(last));
        }
      TEST_METHOD(ReplaceChar)
        {
        using namespace System;
        String test(L"Mark Mohr Was Here");
        String out = test.Replace(L'a', L'i');
        Assert::AreEqual(L"Mirk Mohr Wis Here", out);
        }
      TEST_METHOD(Trim)
        {
        using namespace System;
        String test(L"   Mark Mohr Was Here   ");
        String out = test.Trim();
        Assert::AreEqual(L"Mark Mohr Was Here", out);
        }

      TEST_METHOD(TestFormat)
        {
          {
          Int32 t1(100);
          Assert::AreEqual(L"100", (cstring)String::Format(L"{0}", &t1), L"Single argument.");
          t1 = 3;
          String t2(L"men");
          Assert::AreEqual(L"The 3 wise men.", (cstring)String::Format(L"The {0} wise {1}.", &t1, &t2), "Two arguments.");
          }

          {
          String t1(L"do");
          String t2(L"me");
          String t3(L"so");
          Assert::AreEqual(L"do re me fa so.", String::Format(L"{0} re {1} fa {2}.", &t1, &t2, &t3), L"Three arguments.");
          }
        }

      TEST_METHOD(IndexOfAny1)
        {
        String s(L"abcdefghijklmd");
        wchar_t c[5] = { L'a', L'e', L'i', L'o', L'u'};
        CharArray cArr((wchar_t*)c, 5);

        Assert::AreEqual<int>(0, s.IndexOfAny(cArr), L"#1");
        cArr.Length(2);
        cArr[0] = L'd';
        cArr[1] = L'z';
        Assert::AreEqual<int>(3, s.IndexOfAny(cArr), L"#1");
        cArr.Length(3);
        cArr[0] = L'q';
        cArr[1] = L'm';
        cArr[2] = L'z';
        Assert::AreEqual<int>(12, s.IndexOfAny(cArr), L"#2");
        cArr.Length(0);
        Assert::AreEqual<int>(-1, s.IndexOfAny(cArr), L"#3");
        }
      TEST_METHOD(SubString1)
        {
        String s(L"original");

        Assert::AreEqual(L"inal", (cstring)s.Substring(4), L"#1");
        Assert::AreEqual((cstring)String::Empty(), (cstring)s.Substring(s.Length()), L"#2");
        Assert::AreEqual((cstring)s, (cstring)s.Substring(0), L"#3");
        }
      TEST_METHOD(SplitString)
        {
        StringArray res;
        StringArray in(1);
        in[0] = L"A";

        // count == 0
        res = String(L"A B C").Split(in, 0, StringSplitOptions::None);
        Assert::AreEqual<sizet>(0, res.Length());

        // empty and RemoveEmpty
        res = String::Empty().Split(in, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(0, res.Length());

        // Not found
        in[0] = L"D";
        res = String(L"A B C").Split(in, StringSplitOptions::None);
        Assert::AreEqual<sizet>(1, res.Length());
        Assert::AreEqual(L"A B C", res[0]);

        // A normal test
        in.Length(2);
        in[0] = L"B";
        in[1] = L"D";
        res = String(L"A B C DD E").Split(in, StringSplitOptions::None);
        Assert::AreEqual<sizet>(4, res.Length());
        Assert::AreEqual(L"A ", res[0]);
        Assert::AreEqual(L" C ", res[1]);
        Assert::AreEqual((cstring)String::Empty(), res[2]);
        Assert::AreEqual(L" E", res[3]);

        // Same with RemoveEmptyEntries
        res = String(L"A B C DD E").Split(in, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(3, res.Length());
        Assert::AreEqual(L"A ", res[0]);
        Assert::AreEqual(L" C ", res[1]);
        Assert::AreEqual(L" E", res[2]);

        // Delimiter matches once at the beginning of the string
        in.Length(1);
        in[0] = L"A";
        res = String(L"A B").Split(in, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(1, res.Length());
        Assert::AreEqual(L" B", res[0]);

        // Delimiter at the beginning and at the end
        in[0] = L"B";
        res = String(L"B C DD B").Split(in, StringSplitOptions::None);
        Assert::AreEqual<sizet>(3, res.Length());
        Assert::AreEqual((cstring)String::Empty(), res[0]);
        Assert::AreEqual(L" C DD ", res[1]);
        Assert::AreEqual((cstring)String::Empty(), res[2]);

        res = String(L"B C DD B").Split(in, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(1, res.Length());
        Assert::AreEqual(L" C DD ", res[0]);

        // count
        in.Length(2);
        in[1] = L"D";
        res = String(L"A B C DD E").Split(in, 2, StringSplitOptions::None);
        Assert::AreEqual<sizet>(2, res.Length());
        Assert::AreEqual(L"A ", res[0]);
        Assert::AreEqual(L" C DD E", res[1]);

        // Ordering
        in[0] = L"EF";
        in[1] = L"BCDE";
        res = String(L"ABCDEF").Split(in, StringSplitOptions::None);
        Assert::AreEqual<sizet>(2, res.Length());
        Assert::AreEqual(L"A", res[0]);
        Assert::AreEqual(L"F", res[1]);

        in[0] = L"BCD";
        in[1] = L"BC";
        res = String(L"ABCDEF").Split (in, StringSplitOptions::None);
        Assert::AreEqual<sizet>(2, res.Length());
        Assert::AreEqual(L"A", res[0]);
        Assert::AreEqual(L"EF", res[1]);

        // Whitespace
        in.Length(0);
        res = String(L"A B\nC").Split(in, StringSplitOptions::None);
        Assert::AreEqual<sizet>(3, res.Length());
        Assert::AreEqual(L"A", res[0]);
        Assert::AreEqual(L"B", res[1]);
        Assert::AreEqual(L"C", res[2]);

        res = String(L"A B\nC").Split(in, StringSplitOptions::None);
        Assert::AreEqual<sizet>(3, res.Length());
        Assert::AreEqual(L"A", res[0]);
        Assert::AreEqual(L"B", res[1]);
        Assert::AreEqual(L"C", res[2]);

        }
      TEST_METHOD(SplitStringChars)
        {
        using namespace System;

        // empty
        StringArray res;
        wchar_t* a = L"A";
        CharArray in(a, 1);
        res = String::Empty().Split(in);
        Assert::AreEqual<sizet>(1, res.Length());
        Assert::AreEqual((cstring)String::Empty(), res[0]);

        // empty and RemoveEmpty
        res = String::Empty().Split(in, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(0, res.Length());

        // count == 0
        in[0] = L'.';
        res = String(L"..A..B..").Split(in, 0, StringSplitOptions::None);
        Assert::AreEqual<sizet>(0, res.Length(), L"#01-01");

        // count == 1
        res = String(L"..A..B..").Split(in, 1, StringSplitOptions::None);
        Assert::AreEqual<sizet>(1, res.Length(), L"#02-01");
        Assert::AreEqual(L"..A..B..", res[0], L"#02-02");

        // count == 1 + RemoveEmpty
        res = String(L"..A..B..").Split(in, 1, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(1, res.Length(), L"#03-01");
        Assert::AreEqual(L"..A..B..", res[0], L"#03-02");

        // Strange Case A+B A
        res = String(L"...").Split(in, 1, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(1, res.Length(), L"#ABA-01");
        Assert::AreEqual(L"...", res[0], L"#ABA-02");

        // Strange Case A+B B
        res = String(L"...").Split(in, 2, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(0, res.Length(), L"#ABB-01");

        // Keeping Empties and multiple split chars
        in.Length(2);
        in[1] = ';';
        res = String(L"..A;.B.;").Split(in, StringSplitOptions::None);
        Assert::AreEqual<sizet>(7, res.Length(), L"#04-01");
        Assert::AreEqual((cstring)String::Empty(), res[0], L"#04-02");
        Assert::AreEqual((cstring)String::Empty(), res[1], L"#04-03");
        Assert::AreEqual(L"A", res[2], L"#04-04");
        Assert::AreEqual((cstring)String::Empty(), res[3], L"#04-05");
        Assert::AreEqual(L"B", res[4], L"#04-06");
        Assert::AreEqual((cstring)String::Empty(), res[5], "#04-07");
        Assert::AreEqual((cstring)String::Empty(), res[6], L"#04-08");

        // Trimming (3 tests)
        in.Length(1);
        res = String(L"..A").Split(in, 2, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(1, res.Length(), L"#05-01");
        Assert::AreEqual(L"A", res[0], L"#05-02");

        res = String(L"A..").Split(in, 2, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(1, res.Length(), L"#06-01");
        Assert::AreEqual(L"A", res[0], L"#06-02");

        res = String(L"..A..").Split(in, 2, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(1, res.Length(), L"#07-01");
        Assert::AreEqual(L"A", res[0], L"#07-02");

        // Lingering Tail
        res = String(L"..A..B..").Split(in, 2, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(2, res.Length(), L"#08-01");
        Assert::AreEqual(L"A", res[0], L"#08-02");
        Assert::AreEqual(L"B..", res[1], L"#08-03");

        // Whitespace and Long split chain (removing empty chars)
        in.Length(0);
        res = String(L"  A\tBC\n\rDEF    GHI  ").Split(in, StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(4, res.Length(), L"#09-01");
        Assert::AreEqual(L"A", res[0], L"#09-02");
        Assert::AreEqual(L"BC", res[1], L"#09-03");
        Assert::AreEqual(L"DEF", res[2], L"#09-04");
        Assert::AreEqual(L"GHI", res[3], L"#09-05");

        // Nothing but separators
        in.Length(3);
        in[0] = L'.';
        in[1] = L',';
        in[2] = L';';
        res = String(L"..,.;.,").Split(in,2,StringSplitOptions::RemoveEmptyEntries);
        Assert::AreEqual<sizet>(0, res.Length(), L"#10-01");


        // Complete testseries
        in.Length(1);
        in[0] = L'/';
        StringSplitOptions o = StringSplitOptions::RemoveEmptyEntries;
        Assert::AreEqual(L"hi", String(L"hi").Split(in, o)[0], L"#11-01");
        Assert::AreEqual(L"hi", String(L"hi/").Split(in, o)[0], L"#11-02");
        Assert::AreEqual(L"hi", String(L"/hi").Split(in, o)[0], L"#11-03");

        Assert::AreEqual(L"hi..", String("hi../").Split(in, o)[0], L"#11-04-1");
        Assert::AreEqual(L"hi..", String(L"/hi..").Split(in, o)[0], L"#11-04-2");

        res = String(L"/hi/..").Split(in, o);
        Assert::AreEqual(L"hi", res[0], L"#11-05-1");
        Assert::AreEqual(L"..", res[1], L"#11-05-2");
        Assert::AreEqual<sizet>(2, res.Length(), L"#11-09-3");

        res = String(L"hi/..").Split(in, o);
        Assert::AreEqual(L"hi", res[0], L"#11-06-1");
        Assert::AreEqual(L"..", res[1], L"#11-06-2");
        Assert::AreEqual<sizet>(2, res.Length(), L"#11-09-3");

        res = String("hi/../").Split(in, o);
        Assert::AreEqual(L"hi", res[0], L"#11-07-1");
        Assert::AreEqual(L"..", res[1], L"#11-07-2");
        Assert::AreEqual<sizet>(2, res.Length(), L"#11-07-3");

        res = String("/hi../").Split(in, o);
        Assert::AreEqual(L"hi..", res[0], L"#11-08-1");
        Assert::AreEqual<sizet>(1, res.Length(), L"#11-08-2");

        res = String("/hi/../").Split(in, o);
        Assert::AreEqual(L"hi", res[0], L"#11-09-1");
        Assert::AreEqual(L"..", res[1], L"#11-09-2");
        Assert::AreEqual<sizet>(2, res.Length(), L"#11-09-3");

        in.Length(0);
        Assert::AreEqual<sizet>(0, String("    ").Split(in, 2, StringSplitOptions::RemoveEmptyEntries).Length(), L"#12-00-0");

        in.Length(2);
        in.Set(L'\0');
        res = String(L"not found").Split(in);
        Assert::AreEqual(L"not found", res[0], L"#12-04-27");
        Assert::AreEqual<sizet>(1, res.Length(), L"#12-04-27-A");

        }


      TEST_METHOD(CompareTo)
        {
        using namespace System;
        String test1(L"Mark");
        String test2(L"Mork");
        Assert::IsTrue(test1.CompareTo(test2) != 0);
        }
      TEST_METHOD(TrimEnd)
        {
        using namespace System;
        String test1("Escanaba?????!!!!");
        CharArray sep(2);
        sep[0] = L'?';
        sep[1] = L'!';
        String result = test1.TrimEnd(sep);
        Assert::AreEqual(L"Escanaba", result);
        }
      TEST_METHOD(Join)
        {
        using namespace System;
        StringArray val(3);
        val[0] = L"Mark";
        val[1] = L"Roxy";
        val[2] = L"Caleb";
        String sep(L" - ");
        String out = String::Join(sep, val, 0, (int)val.Length());
        Assert::AreEqual(L"Mark - Roxy - Caleb", out);
        }
      TEST_METHOD(EndsWithTrue)
        {
        using namespace System;
        String town(L"Gaylord, Michigan");
        String state(L"Michigan");
        Assert::IsTrue(town.EndsWith(state));
        }
      TEST_METHOD(EndsWithFalse)
        {
        using namespace System;
        String town(L"Gaylord, Michigan");
        String state(L"Florida");
        Assert::IsFalse(town.EndsWith(state));
        } 
      TEST_METHOD(ToCharArray)
        {
        using namespace System;
        String s(L"original");
        CharArray c;

        c = s.ToCharArray();
        Assert::AreEqual<sizet>(s.Length(), c.Length(), L"#A1");
        Assert::AreEqual((cstring)s, (cstring)String(c), L"#A2");

        c = s.ToCharArray(0, s.Length());
        Assert::AreEqual<sizet>(s.Length(), c.Length(), L"#B1");
        Assert::AreEqual((cstring)s, (cstring)String(c), L"#B2");

        c = s.ToCharArray(1, s.Length() - 1);
        Assert::AreEqual<sizet>(7, c.Length(), L"#C1");
        Assert::AreEqual(L"riginal", (cstring)String(c), L"#C2");

        c = s.ToCharArray(0, 3);
        Assert::AreEqual<sizet>(3, c.Length(), L"#D1");
        Assert::AreEqual(L"ori", (cstring)String(c), L"#D2");

        c = s.ToCharArray(2, 0);
        Assert::AreEqual<sizet>(0, c.Length(), L"#E1");
        Assert::AreEqual((cstring)String::Empty(), (cstring)String(c), L"#E2");

        c = s.ToCharArray(3, 2);
        Assert::AreEqual<sizet>(2, c.Length(), L"#F1");
        Assert::AreEqual(L"gi", (cstring)String(c), L"#F2");

        c = s.ToCharArray(s.Length(), 0);
        Assert::AreEqual<sizet>(0, c.Length(), L"#G1");
        Assert::AreEqual((cstring)String::Empty(), (cstring)String(c), L"#G2");
        } 

      TEST_METHOD(EmptyString)
        {
        using namespace System;
        Assert::AreEqual(String::Empty(), L"");
        }
      TEST_METHOD(Length)
        {
        using namespace System;
        String str(L"test string");
        Assert::AreEqual<int32>(11, str.Length());
        }
      TEST_METHOD(CopyConstructor)
        {
        using namespace System;
        String str1(L"test string");
        String str2(str1);
        Assert::AreEqual((cstring)str2, (cstring)str1);
        }
      TEST_METHOD(AssigmentOperator)
        {
        using namespace System;
        String str1(L"test string1");
        String str2(L"test string2");
        str2 = str1;
        Assert::AreEqual((cstring)str2, (cstring)str1);
        }
    };
  }