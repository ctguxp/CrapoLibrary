#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(UnitTestConvert)
    {
    public:

      TEST_METHOD(TestToBase64CharArray)
        {
        byte byteArr[] = {33, 127, 255, 109, 170, 54};
        //						   0	1	 2	  3    4	5	 6	  7

        wchar_t expectedCharArr[] = {L'I', L'X', L'/', L'/', L'b', L'a', L'o', L'2'};
        CharArray result(8);

        ByteArray in((byte*)byteArr, 6);
        Convert::ToBase64CharArray(in, 0, (int)in.Length(), result, 0);

        for(sizet i = 0; i < 8; i++)
          {
          String msg(L"#S0");
          Int32 d((int)i);
          Assert::AreEqual<wchar_t>(expectedCharArr[i], result[i], (cstring)String::Format(msg, &d));
          }
        }

      TEST_METHOD(FromBase64CharArray)
        {
        wchar_t charArr[] = {L'M',L'o',L'n',L'o',L'm',L'o',L'n',L'o'};
        byte expectedByteArr[] = {50, 137, 232, 154, 137, 232};

        CharArray in((string)charArr, 8);
        ByteArray fromCharArr = Convert::FromBase64CharArray(in, 0, 8);			

        for(sizet i = 0; i < (int)fromCharArr.Length(); i++)
          {
          String msg(L"#U0{0}");
          Int32 d((int)i);
          Assert::AreEqual<byte>(expectedByteArr[i], fromCharArr[i], (cstring)String::Format(msg, &d));
          }
        }

    };
  }