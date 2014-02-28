#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;



namespace corlibtest
  {
  TEST_CLASS(UnitTestUnicode)
    {
    BEGIN_TEST_CLASS_ATTRIBUTE()
      TEST_CLASS_ATTRIBUTE(L"Namespace", L"System.Text")
      END_TEST_CLASS_ATTRIBUTE()

    public:

      TEST_METHOD(TestEncodingGetBytes1)
        {
        using namespace Text;
        //pi and sigma in unicode
        String Unicode = L"\u03a0\u03a3";
        ByteArray UniBytes;
        UnicodeEncoding UnicodeEnc; //little-endian
        UniBytes = UnicodeEnc.GetBytes(Unicode);

        Assert::AreEqual<byte>(0xA0, UniBytes[0], L"Uni #1");
        Assert::AreEqual<byte>(0x03, UniBytes[1], L"Uni #2");
        Assert::AreEqual<byte>(0xA3, UniBytes[2], L"Uni #3");
        Assert::AreEqual<byte>(0x03, UniBytes[3], L"Uni #4");
        }

      TEST_METHOD(TestEncodingGetBytes2)
        {
        using namespace Text;
        //pi and sigma in unicode
        String Unicode = L"\u03a0\u03a3";
        ByteArray UniBytes;
        UnicodeEncoding UnicodeEnc(true, true); //big-endian
        UniBytes = UnicodeEnc.GetBytes(Unicode);

        Assert::AreEqual<byte>(0x03, UniBytes[0], L"Uni #1");
        Assert::AreEqual<byte>(0xA0, UniBytes[1], L"Uni #2");
        Assert::AreEqual<byte>(0x03, UniBytes[2], L"Uni #3");
        Assert::AreEqual<byte>(0xA3, UniBytes[3], L"Uni #4");
        }

      TEST_METHOD(TestEncodingGetBytes3)
        {
        using namespace Text;
        //pi and sigma in unicode
        String Unicode = L"\u03a0\u03a3";
        ByteArray UniBytes(4);
        UnicodeEncoding UnicodeEnc; //little-endian 
        int32 Cnt = UnicodeEnc.GetBytes(Unicode.ToCharArray(), 0, Unicode.Length(), UniBytes, 0);

        Assert::AreEqual<int32>(4, Cnt, L"Uni #1");
        Assert::AreEqual<byte>(0xA0, UniBytes[0], L"Uni #2");
        Assert::AreEqual<byte>(0x03, UniBytes[1], L"Uni #3");
        Assert::AreEqual<byte>(0xA3, UniBytes[2], L"Uni #4");
        Assert::AreEqual<byte>(0x03, UniBytes[3], L"Uni #5");
        }

      TEST_METHOD(TestEncodingDecodingGetBytes1)
        {
        using namespace Text;
        //pi and sigma in unicode
        String Unicode = L"\u03a0\u03a3";
        UnicodeEncoding UnicodeEnc; //little-endian 
        //Encode the unicode string.
        ByteArray UniBytes = UnicodeEnc.GetBytes(Unicode);
        //Decode the bytes to a unicode char array.
        CharArray UniChars = UnicodeEnc.GetChars(UniBytes);
        String Result(UniChars);

        Assert::AreEqual((cstring)Unicode, (cstring)Result, L"Uni #1");
        }

      TEST_METHOD(TestEncodingDecodingGetBytes2)
        {
        using namespace Text;
        //pi and sigma in unicode
        String Unicode = L"\u03a0\u03a3";
        UnicodeEncoding UnicodeEnc(true, true); //big-endian 
        //Encode the unicode string.
        ByteArray UniBytes = UnicodeEnc.GetBytes(Unicode);
        //Decode the bytes to a unicode char array.
        CharArray UniChars = UnicodeEnc.GetChars(UniBytes);
        String Result(UniChars);

        Assert::AreEqual((cstring)Unicode, (cstring)Result, L"Uni #1");
        }

      TEST_METHOD(TestEncodingGetCharCount)
        {
        using namespace Text;
        byte ba[] = {255, 254, 115, 0, 104, 0, 105, 0};
        ByteArray b((byte*)ba, (sizeof(ba) / sizeof(ba[0])) );
        UnicodeEncoding encoding;

        Assert::AreEqual<int32>(3, encoding.GetCharCount(b, 2, (int)b.Length() - 2), L"GetCharCount #1");
        }

      TEST_METHOD(TestPreamble1)
        {
        using namespace Text;
        //litle-endian with byte order mark.
        UnicodeEncoding UnicodeEnc(false, true); 
        ByteArray PreAmble = UnicodeEnc.GetPreamble();

        Assert::AreEqual<byte>(0xFF, PreAmble[0], L"Uni #1");
        Assert::AreEqual<byte>(0xFE, PreAmble[1], L"Uni #2");
        }

      TEST_METHOD(TestPreamble2)
        {
        using namespace Text;
        //big-endian with byte order mark.
        UnicodeEncoding UnicodeEnc(true, true); 
        ByteArray PreAmble = UnicodeEnc.GetPreamble();

        Assert::AreEqual<byte>(0xFE, PreAmble[0], L"Uni #1");
        Assert::AreEqual<byte>(0xFF, PreAmble[1], L"Uni #2");
        }

    };
  }