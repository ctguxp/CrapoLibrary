// Perch Lake Computer System

#include "pch.h"
#include "System.Exception.h"
#include "System.Text.StringBuilder.h"

namespace System
  {
  // Constructor that takes const wchar_t
  Exception::Exception(cstring message)
    :_hresult(-2146233088)
    ,_message( (message != nullptr ? message : L"") )
    ,_innerException(nullptr)
    {
    }
  Exception::Exception(cstring message, Exception& innerException)
    :_hresult(-2146233088)
    ,_message( (message != nullptr ? message : L"") )
    ,_innerException(new Exception(innerException))
    {
    }
  // Copy constructor
  Exception::Exception(const Exception& e)
    :_hresult(e._hresult)
    ,_message(e._message)
    ,_innerException(nullptr)
    {
    if(e._innerException != nullptr)
      _innerException = new Exception(*e._innerException);
    }
  // Destructor
  Exception::~Exception()
    {
    Free();
    }
  // Assignment operator
  Exception& Exception::operator = (const Exception& e)
    {
    if(this == &e)
      return *this;

    _message = e._message;
    _hresult = e._hresult;
    
    if(e._innerException != nullptr)
      _innerException = new Exception(*e._innerException);

    return *this;
    }
  cstring Exception::Message()
    {
    return (cstring)_message;
    }
  void Exception::Message(cstring message)
    {
    _message = message;
    }
  // Get InnerException property
  Exception* Exception::InnerException()
    {
    return _innerException;
    }
  void Exception::Free()
    {
    if(_innerException != nullptr)
      {
      delete _innerException;
      _innerException = nullptr;
      }
    }

  SystemException::SystemException(cstring message)
    :Exception( (message != nullptr ? message : L"A system exception has occurred.") )
    {
    _hresult = -2146233087;
    }

  SystemException::SystemException(cstring message, Exception& innerException)
    :Exception( (message != nullptr ? message : L"A system exception has occurred."), innerException)
    {
    _hresult = -2146233087;
    }

  SystemException::SystemException(const SystemException& e)
    :Exception(e)
    {
    }
  
  SystemException::~SystemException()
    {
    }

  // Assignment operator
  SystemException& SystemException::operator = (const SystemException& e)
    {
    Exception::operator=(e);
    return *this;
    }

  ArgumentException::ArgumentException(cstring message, cstring param)
    :SystemException( (message != nullptr ? message : L"Value does not fall within the expected range.") )
    ,_paramName((param != nullptr) ? param : L"")
    {
    _hresult = -2147024809;
    UpdateMessage();
    }

  ArgumentException::ArgumentException(cstring message, Exception& innerException)
    :SystemException( (message != nullptr ? message : L"Value does not fall within the expected range."), innerException)
    ,_paramName(L"")
    {
    _hresult = -2147024809;
    }

  // Copy constructor
  ArgumentException::ArgumentException(const ArgumentException& e)
    :SystemException(e)
    ,_paramName(e._paramName)
    {
    }

  ArgumentException::~ArgumentException()
    {
    }

  // Assignment operator
  ArgumentException& ArgumentException::operator = (const ArgumentException& e)
    {
    SystemException::operator=(e);
    _paramName = e._paramName;
    return *this;
    }
  
  void ArgumentException::UpdateMessage()
    {
    // Does parameter string have a value
    if(_paramName.Length() != 0)
      {

      System::Text::StringBuilder sb;
      sb.Append(_message);
      // Construct strings
      System::String nl(L"\r\n");     // Replace someday with environment new line;
      System::String pn(L"Parameter name: ");

      sb.Append(nl);
      sb.Append(pn);
      sb.Append(_paramName);

      _message = sb.ToString();
      }
    }

  // Default constructor
  ArgumentNullException::ArgumentNullException(cstring paramName, cstring message)
    :ArgumentException( (message != nullptr ? message : L"Argument cannot be null."), paramName )
    {
    _hresult = -2147467261;
    }
  // Constructor that takes string and innerException
  ArgumentNullException::ArgumentNullException(cstring message, Exception& e)
    :ArgumentException( (message != nullptr ? message : L"Argument cannot be null."), e)
    {
    _hresult = -2147467261;
    }
  // Copy constructor
  ArgumentNullException::ArgumentNullException(const ArgumentNullException& e)
    :ArgumentException(e)
    {
    }
  // Destructor
  ArgumentNullException::~ArgumentNullException()
    {
    }
  // Assignment operator
  ArgumentNullException& ArgumentNullException::operator = (const ArgumentNullException& e)
    {
    ArgumentException::operator=(e);
    return *this;
    }

  // Default constructor
  ArgumentOutOfRangeException::ArgumentOutOfRangeException(cstring paramName, cstring message)
    :ArgumentException( (message != nullptr ? message : L"Argument is out of range."), paramName )
    //,_a2146233086(nullptr)
    {
    _hresult = -2146233086;
    //C_2146233086(L"");
    }
  // Constructor that takes actual value
  //ArgumentOutOfRangeException::ArgumentOutOfRangeException(cstring paramName, cstring actualValue, cstring message)
    //:ArgumentException( (message != nullptr ? message : L"Argument is out of range."), paramName )
    //,_a2146233086(nullptr)
    //{
    //C_2146233086( (actualValue != nullptr ? actualValue : L"") );
    //}
  // Constructor that takes string and innerException
  ArgumentOutOfRangeException::ArgumentOutOfRangeException(cstring message, Exception& e)
    :ArgumentException( (message != nullptr ? message : L"Argument is out of range."), e)
    //,_a2146233086(nullptr)
    {
    _hresult = -2146233086;
     //C_2146233086(L"");
    }
  // Copy constructor
  ArgumentOutOfRangeException::ArgumentOutOfRangeException(const ArgumentOutOfRangeException& e)
    :ArgumentException(e)
    {
    }
  // Destructor
  ArgumentOutOfRangeException::~ArgumentOutOfRangeException()
    {
    //F_2146233086();
    }
  // Assignment operator
  ArgumentOutOfRangeException& ArgumentOutOfRangeException::operator = (const ArgumentOutOfRangeException& e)
    {
    ArgumentException::operator=(e);
    return *this;
    }
  /*void ArgumentOutOfRangeException::F_2146233086()
    {
    if(_a2146233086 != nullptr)
      {
      delete [] _a2146233086;
      _a2146233086 = nullptr;
      }
    }
  void ArgumentOutOfRangeException::C_2146233086(cstring param)
    {
    _a2146233086 = new wchar_t[sys::string::length(param) + 1];
    sys::string::copy(_a2146233086, param);
    }*/

  UnauthorizedAccessException::UnauthorizedAccessException(cstring message)
    :SystemException( (message != nullptr ? message : L"Access to the requested resource is not authorized.") )
    {
    }

  UnauthorizedAccessException::UnauthorizedAccessException(cstring message, Exception& e)
    :SystemException( (message != nullptr ? message : L"Access to the requested resource is not authorized."), e)
    {
    }

  UnauthorizedAccessException::UnauthorizedAccessException(const UnauthorizedAccessException& e)
    :SystemException(e)
    {
    }

  UnauthorizedAccessException::~UnauthorizedAccessException()
    {
    }

  UnauthorizedAccessException& UnauthorizedAccessException::operator = (const UnauthorizedAccessException& e)
    {
    SystemException::operator=(e);
    return *this;
    }


  // Default constructor
  WinException::WinException(cstring message, ulong err)
    :Exception(message)
    ,_err(err)
    {
     using namespace System;
    // Allocate buffer
    CharArray buffer(max_lib_buffer_size + 1);
    ulong retval = ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
                                  ,0, _err, 0, buffer.ToPtr(), max_lib_buffer_size, nullptr);

    if(retval != 0)
      {
      System::Text::StringBuilder sb;
      buffer[retval - 2] = L'\0';
      String temp(L"[");
      sb.Append(temp);
      sb.Append(buffer.ToConstPtr());
      sb.Append(L"]");
      sb.Append(L" - ");
      sb.Append(_message);
      _message = sb.ToString();
      }
    }
  // Destructor
  WinException::~WinException()
    {
    }
  }
