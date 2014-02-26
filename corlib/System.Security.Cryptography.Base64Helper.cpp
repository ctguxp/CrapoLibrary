#include "pch.h"
#include "System.Security.Cryptography.Base64Helper.h"
#include "System.Security.Cryptography.Base64Constants.h"
#include "System.Array.hpp"
#include "System.Exception.h"

namespace System
  {
  namespace Security
    {
    namespace Cryptography
      {
      Base64Helper::Base64Helper()
        {
        }
      Base64Helper::~Base64Helper()
        {
        }
      void Base64Helper::TransformBlock(ByteArray& inputBuffer, int inputOffset, int, ByteArray& outputBuffer, int outputOffset)
        {
        ByteArray lookup((byte*)Base64Constants::EncodeTable, 64);

        int b1 = inputBuffer[inputOffset];
        int b2 = inputBuffer[inputOffset + 1];
        int b3 = inputBuffer[inputOffset + 2];

        outputBuffer[outputOffset] = lookup[b1 >> 2];
        outputBuffer[outputOffset+1] = lookup[((b1 << 4) & 0x30) | (b2 >> 4)];
        outputBuffer[outputOffset+2] = lookup[((b2 << 2) & 0x3c) | (b3 >> 6)];
        outputBuffer[outputOffset+3] = lookup[b3 & 0x3f];
        }
      ByteArray Base64Helper::TransformFinalBlock(ByteArray& inputBuffer, int inputOffset, int inputCount)
        {
        int blockLen = (int)BlockSize::Input;
        int outLen = (int)BlockSize::Output;
        int fullBlocks = inputCount / blockLen;
        int tail = inputCount % blockLen;

        ByteArray res( (inputCount != 0) ? (((inputCount + 2) / blockLen) * outLen) : 0);

        int outputOffset = 0;

        for(int i = 0; i < fullBlocks; i++) 
          {
          TransformBlock(inputBuffer, inputOffset, blockLen, res, outputOffset);
          inputOffset += blockLen;
          outputOffset += outLen;
          }

        ByteArray lookup((byte*)Base64Constants::EncodeTable, 64);
        int b1,b2;

        // When fewer than 24 input bits are available
        // in an input group, zero bits are added
        // (on the right) to form an integral number of
        // 6-bit groups.
        switch(tail)
          {
          case 0:
            break;
          case 1:
            b1 = inputBuffer[inputOffset];
            res[outputOffset] = lookup[b1 >> 2];
            res[outputOffset+1] = lookup[(b1 << 4) & 0x30];

            // padding
            res[outputOffset+2] = (byte)L'=';
            res[outputOffset+3] = (byte)L'=';
            break;

          case 2:
            b1 = inputBuffer[inputOffset];
            b2 = inputBuffer[inputOffset + 1];
            res[outputOffset] = lookup[b1 >> 2];
            res[outputOffset+1] = lookup[((b1 << 4) & 0x30) | (b2 >> 4)];
            res[outputOffset+2] = lookup[(b2 << 2) & 0x3c];

            // one-byte padding
            res [outputOffset+3] = (byte)L'=';
            break;
          }

        return res;
        }

      ByteArray Base64Helper::ToByteArray(wchar_t* start, int length, bool allowWhitespaceOnly)
        {

        wchar_t last, prev_last, prev2_last;
        int a[4], b[4];

        int havePadding = 0;
        int ignored = 0;
        int i = 0;
        wchar_t c = L'\0';
        last = prev_last = 0, prev2_last = 0;
        for(; i < length; i++) 
          {
          c = start[i];
          if(c >= sizeof(Base64Constants::DecodeTable)) 
            {
            throw SystemException(L"Invalid character found.");
            // TODO : Use FormatExcepton
            /*exc = mono_exception_from_name_msg (mono_get_corlib (),
            "System", "FormatException",
            "Invalid character found.");
            mono_raise_exception (exc);*/
            } 
          else if(isspace (c)) 
            {
            ignored++;
            } 
          else if(havePadding && c != '=')
            {
            throw SystemException(L"Invalid character found.");
            // TODO : Use FormatExcepton
            /*exc = mono_exception_from_name_msg (mono_get_corlib (),
            "System", "FormatException",
            "Invalid character found.");
            mono_raise_exception (exc);*/
            } 
          else 
            {
            if(c == '=') havePadding = 1;
            prev2_last = prev_last;
            prev_last = last;
            last = c;
            }
          }

        int olength = length - ignored;
        ByteArray result;
        if(allowWhitespaceOnly && olength == 0) 
          {
          return result;  // Return empty array
          }

        if((olength & 3) != 0 || olength <= 0)
          {
          throw SystemException(L"Invalid length.");
          // TODO : Use FormatExcepton
          /*exc = mono_exception_from_name_msg (mono_get_corlib (), "System",
          "FormatException", "Invalid length.");
          mono_raise_exception (exc)*/;
          }

        if (prev2_last == '=') 
          {
          throw SystemException(L"Invalid format.");
          // TODO : Use FormatExcepton
          /*exc = mono_exception_from_name_msg (mono_get_corlib (), "System", "FormatException", "Invalid format.");
          mono_raise_exception (exc);*/
          }

        olength = (olength * 3) / 4;
        if(last == '=')
          olength--;

        if (prev_last == '=')
          olength--;


        result.Length(olength);
        byte* res_ptr = result.ToPtr();
        for(i = 0; i < length; )
          {
          int k;

          for (k = 0; k < 4 && i < length;) 
            {
            c = start[i++];
            if(isspace(c))
              continue;

            a[k] = (byte)c;
            if(((b[k] = Base64Constants::DecodeTable[c]) & 0x80) != 0)
              {
              throw SystemException(L"Invalid character found.");
              // TODO : Use FormatExcepton
              /*exc = mono_exception_from_name_msg (mono_get_corlib (),
              "System", "FormatException",
              "Invalid character found.");
              mono_raise_exception (exc);*/
              }
            k++;
            }

          *res_ptr++ = (byte)((b[0] << 2) | (b[1] >> 4));
          if(a[2] != L'=')
            *res_ptr++ = (byte)((b[1] << 4) | (b[2] >> 2));
          if(a[3] != L'=')
            *res_ptr++ = (byte)((b[2] << 6) | b[3]);

          while (i < length && isspace (start [i]))
            i++;
          }

        return result;
        }
      }
    }
  }
