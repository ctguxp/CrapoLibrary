// Perch Lake Computer System

#pragma once
#pragma warning(disable:4365)
#pragma warning(disable:4514)
#pragma warning(disable:4365)
#pragma warning(disable:4668)
#pragma warning(disable:4710)
#pragma warning(disable:4711)
#pragma warning(disable:4820)

typedef unsigned char byte;
typedef char sbyte;
typedef unsigned long ulong;
typedef const wchar_t* cstring;
typedef wchar_t* string;

typedef __int8             int8;
typedef unsigned __int8   uint8;
typedef __int16           int16;
typedef unsigned __int16 uint16;
typedef __int32           int32;
typedef unsigned __int32 uint32;
typedef __int64           int64;
typedef unsigned __int64 uint64;

#ifdef _WIN64
typedef uint64 sizet;
typedef int64  intptr;
typedef uint64 uintptr;
#else
typedef uint32 sizet;
typedef int32  intptr;
typedef uint32 uintptr;
#endif


#define idx2string(idx) (locale_strings + (idx))

const int16 max_lib_buffer_size = 32767;  // Default buffer size

#include <windows.h>
#include <commctrl.h>

#include <cassert>
#include <typeinfo>
#include <type_traits>

#ifdef CRAPOCOREDLL_ENABLED
#ifdef CRAPOCOREDLL_EXPORTS
#define CRAPOCOREDLL_API __declspec(dllexport) 
#else
#define CRAPOCOREDLL_API __declspec(dllimport) 
#endif
#else
#define CRAPOCOREDLL_API
#endif
