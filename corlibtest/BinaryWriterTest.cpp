#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace System;

namespace corlibtest
  {
  TEST_CLASS(BinaryWriterTest)
    {
    private:
      String _tempFolder;

      void DeleteFile(String path)
        {
        using namespace IO;
        if(File::Exists(path))
          File::Delete (path);
        }
    public:
      BinaryWriterTest()
        {
        using namespace IO;
        _tempFolder = Path::Combine(Path::GetTempPath (), L"CrapoTests.System.IO.Tests");

        if(Directory::Exists(_tempFolder))
          Directory::Delete(_tempFolder, true);
        Directory::CreateDirectory(_tempFolder);
        }

      ~BinaryWriterTest()
        {
        if(IO::Directory::Exists(_tempFolder))
          IO::Directory::Delete(_tempFolder, true);
        }

      TEST_METHOD(Ctor)
        {
        using namespace IO;
        MemoryStream stream;
        BinaryWriter writer1(&stream);
        Assert::AreEqual<bool>(true, writer1.BaseStream().CanRead(), L"test#01");
        Assert::AreEqual<bool>(true, writer1.BaseStream().CanSeek(), L"test#02");
        Assert::AreEqual<bool>(true, writer1.BaseStream().CanWrite(), L"test#03");

        Text::GCEncoding enc(new Text::ASCIIEncoding());
        BinaryWriter writer2(&stream, enc);
        Assert::AreEqual<bool>(true, writer2.BaseStream().CanRead(), L"test#04");
        Assert::AreEqual<bool>(true, writer2.BaseStream().CanSeek(), L"test#05");
        Assert::AreEqual<bool>(true, writer2.BaseStream().CanWrite(), L"test#06");			
        }

      TEST_METHOD(CtorNullExceptionStream) 
        {
        using namespace IO;
        try
          {
          BinaryWriter reader(nullptr);
          Assert::Fail(L"Should have thrown ArgumentNullException");
          }
        catch(ArgumentNullException&)
          {
          }
        }

      TEST_METHOD(CtorNullExceptionStreamEncoding) 
        {
        using namespace IO;
        try
          {
          MemoryStream stream;
          Text::GCEncoding enc; // nullptr
          BinaryWriter reader(&stream, enc);
          Assert::Fail(L"Should have thrown ArgumentNullException");
          }
        catch(ArgumentNullException&)
          {
          }
        }

      TEST_METHOD(CtorExceptionStreamClosed) 
        {
        using namespace IO;
        try
          {
          MemoryStream stream;
          stream.Close();		
          BinaryWriter writer(&stream);
          Assert::Fail(L"Should have thrown ArgumentException");
          }
        catch(ArgumentException&)
          {
          }
        }

      /*TEST_METHOD(Close1)
        {
        using namespace IO;
        try
          {
          MemoryStream stream;
          Text::GCEncoding enc(new Text::ASCIIEncoding());
          BinaryWriter writer(&stream, enc);
          writer.Close();
          writer.Write(L"Test");
          Assert::Fail(L"Should have thrown ObjectDisposedException");
          }
        catch(ObjectDisposedException& ex)
          {
          }
        }*/

      TEST_METHOD(CtorArgumentExceptionStreamCannotWrite)
        {
        using namespace IO;
        String file = L"/BinaryWriterTest.1";
        String path = _tempFolder + file;
        this->DeleteFile(path);

        try {
          FileStream stream(path, FileMode::OpenOrCreate, FileAccess::Read);
          BinaryWriter reade(&stream);
          Assert::Fail(L"Should have thrown ArgumentException");
          }
        catch(ArgumentException&)
          {
          }
        this->DeleteFile(path);
        }

    };
  }