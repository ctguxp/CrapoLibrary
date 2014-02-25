#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(UnitTestPath)
    {
    public:

      TEST_METHOD(GetRootPath)
        {
        try
          {
          String path(L"c:\\program files\\data\\ralph");
          String root = IO::Path::GetPathRoot(path);
          Assert::AreEqual(L"c:\\", root);
          }
        catch(Exception& ex)
          {
          Assert::Fail(ex.Message());
          }
        }
      TEST_METHOD(GetDirectory)
        {
        try
          {
          String path(L"c:\\program files\\data\\ralph\\jane.txt");
          String root = IO::Path::GetDirectoryName(path);
          Assert::AreEqual(L"c:\\program files\\data\\ralph", root);
          }
        catch(Exception& ex)
          {
          Assert::Fail(ex.Message());
          }
        }
      /*TEST_METHOD(GetFullPath)
        {
        try
          {
          String s2 = Environment::CurrentDirectory();
          String s1(L"C:\\mark");
          Environment::CurrentDirectory(s1);
          String path(L"jane.txt");
          String root = IO::Path::GetFullPath(path);
          Assert::AreEqual(L"c:\\mark\\jane.txt", root, true);
          Environment::CurrentDirectory(s2);
          }
        catch(Exception& ex)
          {
          Assert::Fail(ex.Message());
          }
        }*/

      TEST_METHOD(GetTempPath)
        {
        try
          {
          using namespace IO;
          String getTempPath = Path::GetTempPath();
          Assert::IsTrue(getTempPath != String::Empty(), L"GetTempPath #01");
          Assert::IsTrue(Path::IsPathRooted(getTempPath), L"GetTempPath #02");
          Assert::AreEqual<wchar_t>(Path::DirectorySeparatorChar(), getTempPath[getTempPath.Length() - 1], L"GetTempPath #03");
          }
        catch(Exception& ex)
          {
          Assert::Fail(ex.Message());
          }
        }

    };
  }