#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace corlibtest
  {
	TEST_CLASS(Environment1)
	  {
	  public:
		TEST_METHOD(SetGetDirectory)
		  {
      using namespace System;
      String s1(L"C:\\Program Files");
      String s2 = Environment::CurrentDirectory();
      Assert::AreNotEqual((cstring)s1, (cstring)s2);
      Environment::CurrentDirectory(s1);
      s2 = Environment::CurrentDirectory();
      Assert::AreEqual((cstring)s1, (cstring)s2);
		  }
	  };
  }