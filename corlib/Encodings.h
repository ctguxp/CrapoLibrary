// Perch Lake Computer System

#pragma once
#include "System.String.h"

static const char *encodings [] = {
  (char *) 1,
  "ascii", "us_ascii", "us", "ansi_x3.4_1968",
  "ansi_x3.4_1986", "cp367", "csascii", "ibm367",
  "iso_ir_6", "iso646_us", "iso_646.irv:1991",
  (char *) 2,
  "utf_7", "csunicode11utf7", "unicode_1_1_utf_7",
  "unicode_2_0_utf_7", "x_unicode_1_1_utf_7",
  "x_unicode_2_0_utf_7",
  (char *) 3,
  "utf_8", "unicode_1_1_utf_8", "unicode_2_0_utf_8",
  "x_unicode_1_1_utf_8", "x_unicode_2_0_utf_8",
  (char *) 4,
  "utf_16", "UTF_16LE", "ucs_2", "unicode",
  "iso_10646_ucs2",
  (char *) 5,
  "unicodefffe", "utf_16be",
  (char *) 6,
  "iso_8859_1",
  (char *) 0
  };

System::String* GetCodePage(int& code_page);