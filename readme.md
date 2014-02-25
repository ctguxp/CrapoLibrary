## Crapo Library

### What is it?  Take Mono and convert it to C++ and you get a pile of crap.
### Well not really. In reality anyone who sees this project will say I am full of crap.

I started this to see how hard it would be to develop a .NET style library written in
*unmanaged* code.  This is what I have come up with so far.

**This code should not be used in any major software it is only experimental**

Here is a code snippet

```#include "pch.h"

```using namespace System;

```void main()
```  {
```  try
```    {
```    using namespace IO;
```    StreamReader sr(L"C:\\perch lake\\HowToRead\\test.cs2.TXT");
```    String line = sr.ReadToEnd();
```    wprintf(L"%s\n", (cstring)line);
```    }
```  catch(Exception& ex)
```    {
```    wprintf(L"The file could not be read:\n");
```    wprintf(L"%s\n", ex.Message());
```    }
```  }
