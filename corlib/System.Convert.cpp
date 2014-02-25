// Perch Lake Computer System

#include "pch.h"
#include "System.Convert.h"
#include "System.Array.hpp"

namespace System
  {

  const byte encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  const byte decode_table[] = { 66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
                                66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
                                54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
                                29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
                                66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
                                66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
                                66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
                                66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
                                66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
                                66,66,66,66,66,66 };

  // Default constructor  
  Convert::Convert()
    {
    }
  // Destructor
  Convert::~Convert()
    {
    }
  // From base64 string;
  System::ByteArray Convert::from_base64_string(System::String& strIn)
    {
    size_t needlen = 3 * (strIn.Length() / 4) + 2;

    System::ByteArray arrout(needlen);
    System::ByteArray tempin = strIn.to_byte_array();

    uint32 outleft = (uint32)arrout.Length();
    byte* out = arrout.ToPtr();
    uint32 inlen = strIn.Length();
    byte* in = tempin.ToPtr();
    uint32 outlen = (uint32)arrout.Length();

    while (inlen >= 2)
      {
      if (outleft)
        {
        *out++ = ((decode_table[in[0]] << 2) | (decode_table[in[1]] >> 4));
        outleft--;
        }

      if (inlen == 2)
        break;

      if (in[2] == '=')
        {
        if (inlen != 4)
          break;

        if (in[3] != '=')
          break;

        }
      else
        {
        if (outleft)
          {
          *out++ = (((decode_table[in[1]] << 4) & 0xf0) | (decode_table[in[2]] >> 2));
          outleft--;
          }

        if (inlen == 3)
          break;

        if (in[3] == '=')
          {
          if (inlen != 4)
            break;
          }
        else
          {
          if (outleft)
            {
            *out++ = (((decode_table[in[2]] << 6) & 0xc0) | decode_table[in[3]]);
            outleft--;
            }
          }
        }

      in += 4;
      inlen -= 4;
      }

    outlen -= outleft;

    if (inlen != 0)
      {
      arrout.Length(0);
      return arrout;
      }

    return arrout;
  }
  /*System::String Convert::to_base64_string(System::ByteArray& inArr)
    {
    uint32 inlen = inArr.Length();
    uint32 outlen = 1 + ((((inlen) + 2) / 3) * 4);
    System::ByteArray retval(outlen);

    byte* in = inArr;
    byte* out = retval;

    while (inlen && outlen)
      {
      *out++ = encode_table[(in[0] >> 2) & 0x3f];
      if (!--outlen)
        break;
      *out++ = encode_table[((in[0] << 4) + (--inlen ? in[1] >> 4 : 0)) & 0x3f];
      if (!--outlen)
        break;
      *out++ = (inlen ? encode_table[((in[1] << 2) + (--inlen ? in[2] >> 6 : 0)) & 0x3f] : '=');
      if (!--outlen)
        break;
      *out++ = inlen ? encode_table[in[2] & 0x3f] : '=';
      if (!--outlen)
        break;
      if (inlen)
        inlen--;
      if (inlen)
        in += 3;
      }

    if (outlen)
      *out = '\0';

    return System::String(retval);
    }*/
  }
