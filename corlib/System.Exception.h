// Perch Lake Computer System

#pragma once
#include "System.String.h"

namespace System
  {
  class CRAPOCOREDLL_API Exception : public Object
    {
    public:
      // Default constructor
      Exception(cstring message = nullptr);
      // Constructor that takes string and innerException
      Exception(cstring, Exception&);
      // Copy constructor
      Exception(const Exception&);
      // Destructor
      virtual ~Exception();
      // Assignment operator
      Exception& operator = (const Exception&);
      // Get Message property
      virtual cstring Message();
      virtual void Message(cstring message);
      // Get InnerException property
      Exception* InnerException();
    protected:
      void Free();
    protected:
      int        _hresult;
      String     _message;
      Exception* _innerException;
    };

  // SystemException class
  class CRAPOCOREDLL_API SystemException : public Exception
    {
    public:
      // Default constructor
      SystemException(cstring message = nullptr);
      // Constructor that takes string and innerException
      SystemException(cstring, Exception&);
      // Copy constructor
      SystemException(const SystemException&);
      // Destructor
      ~SystemException();
      // Assignment operator
      SystemException& operator = (const SystemException&);
    };

  // ArgumentException class
  class CRAPOCOREDLL_API ArgumentException : public SystemException
    {
    public:
      // Default constructor
      ArgumentException(cstring message = nullptr, cstring paraName = nullptr);
      // Constructor that takes string and innerException
      ArgumentException(cstring, Exception&);
      // Copy constructor
      ArgumentException(const ArgumentException&);
      // Destructor
      ~ArgumentException();
      // Assignment operator
      ArgumentException& operator = (const ArgumentException&);
      // Get Parameter Name property
      virtual cstring ParamName() const { return (cstring)_paramName; }
    protected:
      void UpdateMessage();
    protected:
      String _paramName;
    };

  // ArgumentNullException class
  class CRAPOCOREDLL_API ArgumentNullException : public ArgumentException
    {
    public:
      // Default constructor
      ArgumentNullException(cstring paramName = nullptr, cstring message = nullptr);
      // Constructor that takes string and innerException
      ArgumentNullException(cstring, Exception&);
      // Copy constructor
      ArgumentNullException(const ArgumentNullException&);
      // Destructor
      ~ArgumentNullException();
      // Assignment operator
      ArgumentNullException& operator = (const ArgumentNullException&);
    };

  // ArgumentOutOfRangeException class
  class CRAPOCOREDLL_API ArgumentOutOfRangeException : public ArgumentException
    {
    public:
      // Default constructor
      ArgumentOutOfRangeException(cstring paramName = nullptr, cstring message = nullptr);
      // Constructor that takes actual value
      //ArgumentOutOfRangeException(cstring, cstring, cstring);
      // Constructor that takes string and innerException
      ArgumentOutOfRangeException(cstring, Exception&);
      // Copy constructor
      ArgumentOutOfRangeException(const ArgumentOutOfRangeException&);
      // Destructor
      ~ArgumentOutOfRangeException();
      // Assignment operator
      ArgumentOutOfRangeException& operator = (const ArgumentOutOfRangeException&);
    protected:
      //void F_2146233086();
      //void C_2146233086(cstring);
    protected:
      //WString _a2146233086;
    };

  class CRAPOCOREDLL_API UnauthorizedAccessException : public SystemException
    {
    public:
    // Default constructor
    UnauthorizedAccessException(cstring message = nullptr);
    // Constructor that takes string and innerException
    UnauthorizedAccessException(cstring, Exception&);
    // Copy constructor
    UnauthorizedAccessException(const UnauthorizedAccessException&);
    // Destructor
    ~UnauthorizedAccessException();
    // Assignment operator
    UnauthorizedAccessException& operator = (const UnauthorizedAccessException&);
    };

  class CRAPOCOREDLL_API WinException : public Exception
    {
    public:
      // Default constructor
      WinException(cstring, ulong err = ::GetLastError());
      // Destructor
      ~WinException();
    private:
      ulong _err;   // last error message
    };
  }
