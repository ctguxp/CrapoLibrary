#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {

  TEST_CLASS(UnitTestStreamReader)
    {
    String _tempFolder;
    String _codeFileName;

    TEST_METHOD_INITIALIZE(SetUp) 
      {
      try
        {
        using namespace IO;
        _tempFolder = IO::Path::Combine(IO::Path::GetTempPath(), L"MonoTests.System.IO.Tests");
        Text::StringBuilder sb(_tempFolder);
        sb.Append(Path::DirectorySeparatorChar(), 1);
        sb.Append(L"AFile.txt");

        _codeFileName = sb.ToString().Trim();

        if(!Directory::Exists(_tempFolder))
          Directory::CreateDirectory(_tempFolder);

        if(!File::Exists(_codeFileName))
          {
          GCStream stream(File::Create(_codeFileName));
          }
        }
      catch(Exception& ex)
        {
        Assert::Fail(ex.Message());
        }
      }

    TEST_METHOD_CLEANUP(TearDown)
      {
      using namespace IO;
      if(Directory::Exists(_tempFolder))
        Directory::Delete(_tempFolder, true);
      }

    TEST_METHOD(TestCtor1)
      {
      using namespace IO;

        {
        bool errorThrown = false;
        try 
          {
          StreamReader r(new FileStream(_codeFileName, FileMode::Open, FileAccess::Write));
          } 
        catch(ArgumentException)
          {
          errorThrown = true;
          }
        Assert::IsTrue(errorThrown, L"no read error not thrown");
        }

      }

    TEST_METHOD(TestCtor2)
      {
      using namespace IO;

        {
        bool errorThrown = false;
        try
          {
          StreamReader sr(String::Empty());
          } 
        catch(ArgumentException&)
          {
          errorThrown = true;
          } 
        catch(Exception& ex) 
          {
          Assert::Fail(ex.Message());
          }
        Assert::IsTrue (errorThrown, L"empty string error not thrown");
        }

        {
        bool errorThrown = false;
        try
          {
          new StreamReader(L"nonexistentfile");
          } 
        catch(FileNotFoundException&)
          {
          errorThrown = true;
          } 
        catch(Exception& ex) 
          {
          Assert::Fail(ex.Message());
          }
        Assert::IsTrue (errorThrown, L"fileNotFound error not thrown");
        }

        {
        bool errorThrown = false;
        try 
          {
          new StreamReader(L"nonexistentdir/file");
          } 
        catch(DirectoryNotFoundException&)
          {
          errorThrown = true;
          } 
        catch(Exception ex) 
          {
          String m(ex.Message());
          String msg(L"Incorrect exception thrown at 4: {0}");
          Assert::Fail((cstring)String::Format(msg, &m));
          }
        Assert::IsTrue(errorThrown, L"dirNotFound error not thrown");
        }

        {
        bool errorThrown = false;
        try 
          {
          Text::StringBuilder sb(L"!$what? what? Huh? !$*#");
          sb.Append(Path::InvalidPathChars[0]);
          new StreamReader(sb.ToString());
          } 
        catch(IOException&) 
          {
          errorThrown = true;
          } 
        catch(ArgumentException&) 
          {
          // FIXME - the spec says 'IOExc', but the
          //   compiler says 'ArgExc'...
          errorThrown = true;
          } 
        catch(Exception ex) 
          {
          String m(ex.Message());
          String msg(L"Incorrect exception thrown at 5: {0}");
          Assert::Fail((cstring)String::Format(msg, &m));
          }
        Assert::IsTrue(errorThrown, L"invalid filename error not thrown");
        }
      }
    };
  }