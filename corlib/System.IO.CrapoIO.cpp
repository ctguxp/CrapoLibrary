// Perch Lake Computer System

#include "pch.h"
#include "System.IO.CrapoIO.h"
#include "System.Exception.h"
#include "System.IO.Path.h"
#include "System.Collections.Generic.List.h"
#include "System.Collections.Generic.List.hpp"

namespace System
  {
  namespace IO
    {
    CrapoIO::CrapoIO()
      {
      }
    CrapoIO::~CrapoIO()
      {
      }

    /// Directory Methods
    bool CrapoIO::CreateDirectory(String& path, ulong& error)
      {
      error = ERROR_SUCCESS;
      BOOL ret = ::CreateDirectory((cstring)path, NULL);
      if(ret == FALSE)
        {
        error = ::GetLastError();
        return false;
        }
      return true;
      }

    bool CrapoIO::RemoveDirectory(String& path, ulong& error)
      {
      error = ERROR_SUCCESS;
      BOOL ret = ::RemoveDirectory((cstring)path);
      if(ret == FALSE)
        {
        error = ::GetLastError();
        return false;
        }
      return true;
      }

    String* CrapoIO::GetCurrentDirectory(ulong& error)
      {
      String* result = nullptr;
      error = ERROR_SUCCESS;
      // Allocate buffer
      CharArray buffer(max_lib_buffer_size + 1);
      ulong ret = ::GetCurrentDirectory(max_lib_buffer_size, buffer.ToPtr());
      if(ret != 0)
        {
        buffer[ret] = L'\0';
        result = new String(buffer.ToConstPtr());
        }
      else
        {
        error = ::GetLastError();
        }
      return result;
      }

    bool CrapoIO::SetCurrentDirectory(String& path, ulong& error)
      {
      error = ERROR_SUCCESS;
      BOOL ret = ::SetCurrentDirectory((cstring)path);
      if(ret == FALSE)
        {
        error = ::GetLastError();
        return false;
        }
      return true;
      }


    bool CrapoIO::MoveFile(String& path, String& dest, ulong& error)
      {
      error = ERROR_SUCCESS;
      BOOL ret = ::MoveFile((cstring)path, (cstring)dest);
      if(ret == FALSE)
        {
        error = ::GetLastError();
        return false;
        }
      return true;
      }

    bool CrapoIO::CopyFile(String& path, String& dest, bool overwrite, ulong& error)
      {
      error = ERROR_SUCCESS;
      BOOL ret = ::CopyFile((cstring)path, (cstring)dest, !overwrite);
      if(ret == FALSE)
        {
        error = ::GetLastError();
        return false;
        }
      return true;
      }

    bool CrapoIO::DeleteFile(String& path, ulong& error)
      {
      error = ERROR_SUCCESS;
      BOOL ret = ::DeleteFile((cstring)path);
      if(ret == FALSE)
        {
        error = ::GetLastError();
        return false;
        }
      return true;
      }

    FileAttributes CrapoIO::GetFileAttributes(String& path, ulong& error)
      {
      error = ERROR_SUCCESS;
      int ret = CrapoIO::GetFileAttributes(path);
      if(ret == -1)
        error = ::GetLastError();
      return (FileAttributes)ret;
      }

    bool CrapoIO::Exists(String& path, ulong& error)
      {
      error = ERROR_SUCCESS;
      FileAttributes attrs = CrapoIO::GetFileAttributes (path, error);
      if(attrs == FileAttributes::Invalid)
        return false;

      return true;
      }

    bool CrapoIO::ExistsFile(String& path, ulong& error)
      {
      error = ERROR_SUCCESS;
      FileAttributes attrs = CrapoIO::GetFileAttributes(path, error);
      if(attrs == FileAttributes::Invalid)
        return false;

      if(((intptr)attrs & (intptr)FileAttributes::Directory) != 0)
        return false;

      return true;
      }

    bool CrapoIO::ExistsDirectory(String& path, ulong& error)
      {
      FileAttributes attrs = CrapoIO::GetFileAttributes(path, error);

      // Actually, we are looking for a directory, not a file
      if(error == ERROR_FILE_NOT_FOUND)
        error = ERROR_PATH_NOT_FOUND;

      if(attrs == FileAttributes::Invalid)
        return false;

      if(((ulong)attrs & (ulong)FileAttributes::Directory) == 0)
        return false;

      return true;
      }

    bool CrapoIO::ExistsSymlink(String& path, ulong& error)
      {
      FileAttributes attrs = GetFileAttributes(path, error);
      if(attrs == FileAttributes::Invalid)
        return false;

      if(((intptr)attrs & (intptr)FileAttributes::ReparsePoint) == 0)
        return false;

      return true;
      }

    StringArray CrapoIO::GetFileSystemEntries(String& path, String& path_with_pattern, int attrs, int mask, ulong& error)
      {
      using namespace Collections;
      StringArray result;
      error = ERROR_SUCCESS;

      FileAttributes attribs = CrapoIO::GetFileAttributes(path, error);
      if(attribs != FileAttributes::Invalid)
        {
        if( ((intptr)attribs & (intptr)FileAttributes::Directory) == 0)
          {
          error = ERROR_INVALID_NAME;
          return result;
          }
        }
      else
        {
        error = ::GetLastError();
        return result;
        }

      WIN32_FIND_DATA data;
      HANDLE find_handle = ::FindFirstFile(path_with_pattern, &data);
      if(find_handle == INVALID_HANDLE_VALUE)
        {
        ulong find_error = ::GetLastError();
        if(find_error == ERROR_FILE_NOT_FOUND || find_error == ERROR_NO_MORE_FILES)
          return result;
        error = find_error;
        return result;
        }

      String uPath = Path::GetDirectoryName(path_with_pattern);
      Generic::List<String> names;
      do{
        if(data.cFileName[0] == L'.' && data.cFileName[1] == L'\0' ||
          data.cFileName[0] == L'.' && data.cFileName[1] == L'.' && data.cFileName[2] == L'\0')
          continue;

        String fname;
        if((data.dwFileAttributes & (DWORD)mask) == (DWORD)attrs)
          {
          fname = data.cFileName;
          }
        if(fname.Length() != 0)
          if(!attrs)
            names.Add(Path::Combine(path, String(data.cFileName)));
          else
            names.Add(String(data.cFileName));
        }while(::FindNextFile(find_handle,&data));

      if(FindClose(find_handle) == FALSE)
        {
        error = ::GetLastError();
        return result;
        }
      else
        {
        result = names.ToArray();
        }
     return result;
      }


    HANDLE CrapoIO::Open(String& filename, FileMode mode, FileAccess access, FileShare share, FileOptions options, ulong& error)
      {
      error = ERROR_SUCCESS;
      int attributes;
      FileAttributes attrs;
      if (options != 0)
        {
        if(options & FileOptions::Encrypted)
          attributes = FILE_ATTRIBUTE_ENCRYPTED;
        else
          attributes = FILE_ATTRIBUTE_NORMAL;
        if (options & FileOptions::DeleteOnClose)
          attributes |= FILE_FLAG_DELETE_ON_CLOSE;
        if (options & FileOptions::SequentialScan)
          attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
        if (options & FileOptions::RandomAccess)
          attributes |= FILE_FLAG_RANDOM_ACCESS;

        if (options & FileOptions::Temporary)
          attributes |= FILE_ATTRIBUTE_TEMPORARY;

        /* Not sure if we should set FILE_FLAG_OVERLAPPED, how does this mix with the "Async" bool here? */
        if (options & FileOptions::Asynchronous)
          attributes |= FILE_FLAG_OVERLAPPED;

        if (options & FileOptions::WriteThrough)
          attributes |= FILE_FLAG_WRITE_THROUGH;
        } 
      else
        attributes = FILE_ATTRIBUTE_NORMAL;

      attrs = (FileAttributes)CrapoIO::GetFileAttributes(filename);
      if(attrs != FileAttributes::Invalid) 
        {
        if((ulong)attrs & (ulong)FileAttributes::Directory)
          {
          attributes |= FILE_FLAG_BACKUP_SEMANTICS;
          }
        }
      HANDLE ret = ::CreateFile((cstring)filename, (ulong)access, (ulong)share, NULL, (ulong)mode, attributes, NULL);
      if(ret != INVALID_HANDLE_VALUE)
        {
        return ret;
        }
      error = ::GetLastError();
      return NULL;
      }

    bool CrapoIO::Close(HANDLE handle, ulong& error)
      {
      error = ERROR_SUCCESS;
      BOOL ret = ::CloseHandle(handle);
      if(ret == FALSE)
        {
        error = ::GetLastError();
        return false;
        }
      return true;
      }

    int CrapoIO::Read(HANDLE handle, ByteArray& dest, int dest_offset, int count, ulong& error)
      {
      ulong n;
      if(dest.IsNull())
        throw ArgumentNullException(L"dest");
      if(dest_offset > ((int)dest.Length() - count))
        throw ArgumentOutOfRangeException(L"dest", L"Argument index is out of array range.");
      error = ERROR_SUCCESS;
      byte* buffer = dest.ToPtr();
      BOOL ret = ::ReadFile(handle, buffer, count, &n, NULL);
      if(!ret)
        {
        error = ::GetLastError();
        return -1;
        }
      return (int)n;
      }

    int CrapoIO::GetFileAttributes(String& path)
      {
      int res = ::GetFileAttributes((cstring)path);
      if(res != -1)
        return res;

      ulong error = ::GetLastError();
      if(error != ERROR_SHARING_VIOLATION)
        return res;

      WIN32_FIND_DATA findData;
      HANDLE findHandle = ::FindFirstFile((cstring)path, &findData);
      if(findHandle == INVALID_HANDLE_VALUE)
        return res;

      ::FindClose(findHandle);
      return findData.dwFileAttributes;
      }

    bool CrapoIO::GetFileStat(String& path, CrapoIOStat& stat, ulong& error)
      {
      error = ERROR_SUCCESS;

      WIN32_FILE_ATTRIBUTE_DATA data;
      BOOL retval = get_file_attributes_ex(path, data);

      if(retval)
        {
        stat.Attributes = (FileAttributes)data.dwFileAttributes;
        stat.CreationTime = (long)convert_filetime(data.ftCreationTime);
        stat.LastAccessTime = (long)convert_filetime(data.ftLastAccessTime);
        stat.LastWriteTime = (long)convert_filetime(data.ftLastWriteTime);
        stat.Length = ((int64)data.nFileSizeHigh << 32) | data.nFileSizeLow;
        } 
      else 
        {
        error=GetLastError();
        memset(&stat, 0, sizeof(CrapoIOStat));
        return false;
        }

      return true;;
      }

    CrapoFileType CrapoIO::GetFileType(HANDLE handle, ulong& error)
      {
      error = ERROR_SUCCESS;
      BOOL ret = ::GetFileType(handle);
      if(ret == FILE_TYPE_UNKNOWN)
        {
        error = ::GetLastError();
        return CrapoFileType::Unknown;
        }
      return (CrapoFileType)ret;
      }

    intptr CrapoIO::GetLength(HANDLE handle, ulong& error)
      {
      int64 length = 0;
      uint32 length_hi = 0;
      error = ERROR_SUCCESS;
      length = ::GetFileSize(handle, (LPDWORD)&length_hi);
      if(length==INVALID_FILE_SIZE)
        {
        error=GetLastError();
        }

      return (intptr)length | ((int64)length_hi << 32);
      }

    intptr CrapoIO::Seek(HANDLE handle, int64 offset, SeekOrigin origin, ulong& error)
      {
      int32 offset_hi = 0;
      error = ERROR_SUCCESS;
      offset_hi = offset >> 32;
      offset = ::SetFilePointer(handle, (int32)(offset & 0xFFFFFFFF), (PLONG)&offset_hi, (DWORD)origin);
      if(offset == INVALID_SET_FILE_POINTER)
        {
        error=GetLastError();
        }
      return (intptr)offset | ((int64)offset_hi << 32); 
      }

    int CrapoIO::Write(HANDLE handle, ByteArray& src, int src_offset, int count, ulong& error)
      {
      if(src.IsNull())
        throw ArgumentNullException(L"src");

      if(src_offset > (int)src.Length() - count)
        throw ArgumentException(L"array", L"array too small. numBytes/offset wrong.");

      error=ERROR_SUCCESS;

      byte* buffer = src.ToPtr();	
      uint32 n;
      BOOL result = WriteFile (handle, buffer, count, (LPDWORD)&n, NULL);

      if (!result)
        {
        error=GetLastError ();
        return -1;
        }

      return (int32)n;
      }

    BOOL CrapoIO::get_file_attributes_ex(String& path, WIN32_FILE_ATTRIBUTE_DATA& data)
      {
      WIN32_FIND_DATA find_data;

      BOOL res = GetFileAttributesEx((cstring)path, GetFileExInfoStandard, &data);
      if(res)
        return TRUE;

      ulong error = GetLastError();

      if(error != ERROR_SHARING_VIOLATION)
        return FALSE;

      HANDLE find_handle = FindFirstFile (path, &find_data);

      if(find_handle == INVALID_HANDLE_VALUE)
        return FALSE;

      FindClose(find_handle);

      data.dwFileAttributes = find_data.dwFileAttributes;
      data.ftCreationTime = find_data.ftCreationTime;
      data.ftLastAccessTime = find_data.ftLastAccessTime;
      data.ftLastWriteTime = find_data.ftLastWriteTime;
      data.nFileSizeHigh = find_data.nFileSizeHigh;
      data.nFileSizeLow = find_data.nFileSizeLow;

      return TRUE;
      }

    int64 CrapoIO::convert_filetime(const FILETIME& filetime)
      {
      uint64 ticks = filetime.dwHighDateTime;
      ticks <<= 32;
      ticks += filetime.dwLowDateTime;
      return (int64)ticks;
      }

    }
  }
