#include "pch.h"
#include "System.Text.RegularExpressions.Category.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      bool CategoryUtils::IsCategory(Globalization::UnicodeCategory uc, wchar_t c)
        {
        if(Char::GetUnicodeCategory (c) == uc)
          return true;

        return false;
        }

      bool CategoryUtils::IsCategory(Category cat, wchar_t c)
        {
        using namespace Globalization;
        switch (cat)
          {
          case Category::None:
            return false;

          case Category::Any:
            return c != L'\n';

          case Category::AnySingleline:
            return true;

          case Category::Word:
            return Char::IsLetterOrDigit (c) || IsCategory(UnicodeCategory::ConnectorPunctuation, c);

          case Category::Digit:
            return Char::IsDigit (c);

          case Category::WhiteSpace:
            return Char::IsWhiteSpace (c);

            // ECMA categories

          case Category::EcmaAny:
            return c != L'\n';

          case Category::EcmaAnySingleline:
            return true;

          case Category::EcmaWord:
            return
              L'a' <= c && c <= L'z' ||
              L'A' <= c && c <= L'Z' ||
              L'0' <= c && c <= L'9' ||
              L'_' == c;

          case Category::EcmaDigit:
            return
              L'0' <= c && c <= L'9';

          case Category::EcmaWhiteSpace:
            return
              c == L' '  ||
              c == L'\f' ||
              c == L'\n' ||
              c == L'\r' ||
              c == L'\t' ||
              c == L'\v';

              // Unicode categories...

              // letter

          case Category::UnicodeLu: return IsCategory(UnicodeCategory::UppercaseLetter, c);
          case Category::UnicodeLl: return IsCategory(UnicodeCategory::LowercaseLetter, c);
          case Category::UnicodeLt: return IsCategory(UnicodeCategory::TitlecaseLetter, c);
          case Category::UnicodeLm: return IsCategory(UnicodeCategory::ModifierLetter, c);
          case Category::UnicodeLo: return IsCategory(UnicodeCategory::OtherLetter, c);

            // mark

          case Category::UnicodeMn: return IsCategory(UnicodeCategory::NonSpacingMark, c);
          case Category::UnicodeMe: return IsCategory(UnicodeCategory::EnclosingMark, c);
          case Category::UnicodeMc: return IsCategory(UnicodeCategory::SpacingCombiningMark, c);
          case Category::UnicodeNd: return IsCategory(UnicodeCategory::DecimalDigitNumber, c);

            // number

          case Category::UnicodeNl: return IsCategory(UnicodeCategory::LetterNumber, c);
          case Category::UnicodeNo: return IsCategory(UnicodeCategory::OtherNumber, c);

            // separator

          case Category::UnicodeZs: return IsCategory(UnicodeCategory::SpaceSeparator, c);
          case Category::UnicodeZl: return IsCategory(UnicodeCategory::LineSeparator, c);
          case Category::UnicodeZp: return IsCategory(UnicodeCategory::ParagraphSeparator, c);

            // punctuation

          case Category::UnicodePd: return IsCategory(UnicodeCategory::DashPunctuation, c);
          case Category::UnicodePs: return IsCategory(UnicodeCategory::OpenPunctuation, c);
          case Category::UnicodePi: return IsCategory(UnicodeCategory::InitialQuotePunctuation, c);
          case Category::UnicodePe: return IsCategory(UnicodeCategory::ClosePunctuation, c);
          case Category::UnicodePf: return IsCategory(UnicodeCategory::FinalQuotePunctuation, c);
          case Category::UnicodePc: return IsCategory(UnicodeCategory::ConnectorPunctuation, c);
          case Category::UnicodePo: return IsCategory(UnicodeCategory::OtherPunctuation, c);

            // symbol

          case Category::UnicodeSm: return IsCategory(UnicodeCategory::MathSymbol, c);
          case Category::UnicodeSc: return IsCategory(UnicodeCategory::CurrencySymbol, c);
          case Category::UnicodeSk: return IsCategory(UnicodeCategory::ModifierSymbol, c);
          case Category::UnicodeSo: return IsCategory(UnicodeCategory::OtherSymbol, c);

            // other

          case Category::UnicodeCc: return IsCategory(UnicodeCategory::Control, c);
          case Category::UnicodeCf: return IsCategory(UnicodeCategory::Format, c);
          case Category::UnicodeCo: return IsCategory(UnicodeCategory::PrivateUse, c);
          case Category::UnicodeCs: return IsCategory(UnicodeCategory::Surrogate, c);
          case Category::UnicodeCn: return IsCategory(UnicodeCategory::OtherNotAssigned, c); 

          case Category::UnicodeL:	// letter
            return
              IsCategory(UnicodeCategory::UppercaseLetter, c) ||
              IsCategory(UnicodeCategory::LowercaseLetter, c) ||
              IsCategory(UnicodeCategory::TitlecaseLetter, c) ||
              IsCategory(UnicodeCategory::ModifierLetter, c) ||
              IsCategory(UnicodeCategory::OtherLetter, c);

          case Category::UnicodeM:	// mark
            return
              IsCategory(UnicodeCategory::NonSpacingMark, c) ||
              IsCategory(UnicodeCategory::EnclosingMark, c) ||
              IsCategory(UnicodeCategory::SpacingCombiningMark, c);

          case Category::UnicodeN:	// number
            return
              IsCategory(UnicodeCategory::DecimalDigitNumber, c) ||
              IsCategory(UnicodeCategory::LetterNumber, c) ||
              IsCategory(UnicodeCategory::OtherNumber, c);

          case Category::UnicodeZ:	// separator
            return
              IsCategory(UnicodeCategory::SpaceSeparator, c) ||
              IsCategory(UnicodeCategory::LineSeparator, c) ||
              IsCategory(UnicodeCategory::ParagraphSeparator, c);

          case Category::UnicodeP:	// punctuation
            return
              IsCategory(UnicodeCategory::DashPunctuation, c) ||
              IsCategory(UnicodeCategory::OpenPunctuation, c) ||
              IsCategory(UnicodeCategory::InitialQuotePunctuation, c) ||
              IsCategory(UnicodeCategory::ClosePunctuation, c) ||
              IsCategory(UnicodeCategory::FinalQuotePunctuation, c) ||
              IsCategory(UnicodeCategory::ConnectorPunctuation, c) ||
              IsCategory(UnicodeCategory::OtherPunctuation, c);

          case Category::UnicodeS:	// symbol
            return
              IsCategory(UnicodeCategory::MathSymbol, c) ||
              IsCategory(UnicodeCategory::CurrencySymbol, c) ||
              IsCategory(UnicodeCategory::ModifierSymbol, c) ||
              IsCategory(UnicodeCategory::OtherSymbol, c);

          case Category::UnicodeC:	// other
            return
              IsCategory(UnicodeCategory::Control, c) ||
              IsCategory(UnicodeCategory::Format, c) ||
              IsCategory(UnicodeCategory::PrivateUse, c) ||
              IsCategory(UnicodeCategory::Surrogate, c) ||
              IsCategory(UnicodeCategory::OtherNotAssigned, c);

            // Unicode block ranges...

          case Category::UnicodeBasicLatin:
            return 0x0000 <= c && c <= 0x007F;

          case Category::UnicodeLatin1Supplement:
            return 0x0080<= c && c <= 0x00FF;

          case Category::UnicodeLatinExtendedA:
            return 0x0100<= c && c <= 0x017F;

          case Category::UnicodeLatinExtendedB:
            return 0x0180<= c && c <= 0x024F;

          case Category::UnicodeIPAExtensions:
            return 0x0250<= c && c <= 0x02AF;

          case Category::UnicodeSpacingModifierLetters:
            return 0x02B0<= c && c <= 0x02FF;

          case Category::UnicodeCombiningDiacriticalMarks:
            return 0x0300<= c && c <= 0x036F;

          case Category::UnicodeGreek:
            return 0x0370<= c && c <= 0x03FF;

          case Category::UnicodeCyrillic:
            return 0x0400<= c && c <= 0x04FF;

          case Category::UnicodeArmenian:
            return 0x0530<= c && c <= 0x058F;

          case Category::UnicodeHebrew:
            return 0x0590<= c && c <= 0x05FF;

          case Category::UnicodeArabic:
            return 0x0600<= c && c <= 0x06FF;

          case Category::UnicodeSyriac:
            return 0x0700<= c && c <= 0x074F;

          case Category::UnicodeThaana:
            return 0x0780<= c && c <= 0x07BF;

          case Category::UnicodeDevanagari:
            return 0x0900<= c && c <= 0x097F;

          case Category::UnicodeBengali:
            return 0x0980<= c && c <= 0x09FF;

          case Category::UnicodeGurmukhi:
            return 0x0A00<= c && c <= 0x0A7F;

          case Category::UnicodeGujarati:
            return 0x0A80<= c && c <= 0x0AFF;

          case Category::UnicodeOriya:
            return 0x0B00<= c && c <= 0x0B7F;

          case Category::UnicodeTamil:
            return 0x0B80<= c && c <= 0x0BFF;

          case Category::UnicodeTelugu:
            return 0x0C00<= c && c <= 0x0C7F;

          case Category::UnicodeKannada:
            return 0x0C80<= c && c <= 0x0CFF;

          case Category::UnicodeMalayalam:
            return 0x0D00<= c && c <= 0x0D7F;

          case Category::UnicodeSinhala:
            return 0x0D80<= c && c <= 0x0DFF;

          case Category::UnicodeThai:
            return 0x0E00<= c && c <= 0x0E7F;

          case Category::UnicodeLao:
            return 0x0E80<= c && c <= 0x0EFF;

          case Category::UnicodeTibetan:
            return 0x0F00<= c && c <= 0x0FFF;

          case Category::UnicodeMyanmar:
            return 0x1000<= c && c <= 0x109F;

          case Category::UnicodeGeorgian:
            return 0x10A0<= c && c <= 0x10FF;

          case Category::UnicodeHangulJamo:
            return 0x1100<= c && c <= 0x11FF;

          case Category::UnicodeEthiopic:
            return 0x1200<= c && c <= 0x137F;

          case Category::UnicodeCherokee:
            return 0x13A0<= c && c <= 0x13FF;

          case Category::UnicodeUnifiedCanadianAboriginalSyllabics:
            return 0x1400<= c && c <= 0x167F;

          case Category::UnicodeOgham:
            return 0x1680<= c && c <= 0x169F;

          case Category::UnicodeRunic:
            return 0x16A0<= c && c <= 0x16FF;

          case Category::UnicodeKhmer:
            return 0x1780<= c && c <= 0x17FF;

          case Category::UnicodeMongolian:
            return 0x1800<= c && c <= 0x18AF;

          case Category::UnicodeLatinExtendedAdditional:
            return 0x1E00<= c && c <= 0x1EFF;

          case Category::UnicodeGreekExtended:
            return 0x1F00<= c && c <= 0x1FFF;

          case Category::UnicodeGeneralPunctuation:
            return 0x2000<= c && c <= 0x206F;

          case Category::UnicodeSuperscriptsandSubscripts:
            return 0x2070<= c && c <= 0x209F;

          case Category::UnicodeCurrencySymbols:
            return 0x20A0<= c && c <= 0x20CF;

          case Category::UnicodeCombiningMarksforSymbols:
            return 0x20D0<= c && c <= 0x20FF;

          case Category::UnicodeLetterlikeSymbols:
            return 0x2100<= c && c <= 0x214F;

          case Category::UnicodeNumberForms:
            return 0x2150<= c && c <= 0x218F;

          case Category::UnicodeArrows:
            return 0x2190<= c && c <= 0x21FF;

          case Category::UnicodeMathematicalOperators:
            return 0x2200<= c && c <= 0x22FF;

          case Category::UnicodeMiscellaneousTechnical:
            return 0x2300<= c && c <= 0x23FF;

          case Category::UnicodeControlPictures:
            return 0x2400<= c && c <= 0x243F;

          case Category::UnicodeOpticalCharacterRecognition:
            return 0x2440<= c && c <= 0x245F;

          case Category::UnicodeEnclosedAlphanumerics:
            return 0x2460<= c && c <= 0x24FF;

          case Category::UnicodeBoxDrawing:
            return 0x2500<= c && c <= 0x257F;

          case Category::UnicodeBlockElements:
            return 0x2580<= c && c <= 0x259F;

          case Category::UnicodeGeometricShapes:
            return 0x25A0<= c && c <= 0x25FF;

          case Category::UnicodeMiscellaneousSymbols:
            return 0x2600<= c && c <= 0x26FF;

          case Category::UnicodeDingbats:
            return 0x2700<= c && c <= 0x27BF;

          case Category::UnicodeBraillePatterns:
            return 0x2800<= c && c <= 0x28FF;

          case Category::UnicodeCJKRadicalsSupplement:
            return 0x2E80<= c && c <= 0x2EFF;

          case Category::UnicodeKangxiRadicals:
            return 0x2F00<= c && c <= 0x2FDF;

          case Category::UnicodeIdeographicDescriptionCharacters:
            return 0x2FF0<= c && c <= 0x2FFF;

          case Category::UnicodeCJKSymbolsandPunctuation:
            return 0x3000<= c && c <= 0x303F;

          case Category::UnicodeHiragana:
            return 0x3040<= c && c <= 0x309F;

          case Category::UnicodeKatakana:
            return 0x30A0<= c && c <= 0x30FF;

          case Category::UnicodeBopomofo:
            return 0x3100<= c && c <= 0x312F;

          case Category::UnicodeHangulCompatibilityJamo:
            return 0x3130<= c && c <= 0x318F;

          case Category::UnicodeKanbun:
            return 0x3190<= c && c <= 0x319F;

          case Category::UnicodeBopomofoExtended:
            return 0x31A0<= c && c <= 0x31BF;

          case Category::UnicodeEnclosedCJKLettersandMonths:
            return 0x3200<= c && c <= 0x32FF;

          case Category::UnicodeCJKCompatibility:
            return 0x3300<= c && c <= 0x33FF;

          case Category::UnicodeCJKUnifiedIdeographsExtensionA:
            return 0x3400<= c && c <= 0x4DB5;

          case Category::UnicodeCJKUnifiedIdeographs:
            return 0x4E00<= c && c <= 0x9FFF;

          case Category::UnicodeYiSyllables:
            return 0xA000<= c && c <= 0xA48F;

          case Category::UnicodeYiRadicals:
            return 0xA490<= c && c <= 0xA4CF;

          case Category::UnicodeHangulSyllables:
            return 0xAC00<= c && c <= 0xD7A3;

          case Category::UnicodeHighSurrogates:
            return 0xD800<= c && c <= 0xDB7F;

          case Category::UnicodeHighPrivateUseSurrogates:
            return 0xDB80<= c && c <= 0xDBFF;

          case Category::UnicodeLowSurrogates:
            return 0xDC00<= c && c <= 0xDFFF;

          case Category::UnicodePrivateUse:
            return 0xE000<= c && c <= 0xF8FF;

          case Category::UnicodeCJKCompatibilityIdeographs:
            return 0xF900<= c && c <= 0xFAFF;

          case Category::UnicodeAlphabeticPresentationForms:
            return 0xFB00<= c && c <= 0xFB4F;

          case Category::UnicodeArabicPresentationFormsA:
            return 0xFB50<= c && c <= 0xFDFF;

          case Category::UnicodeCombiningHalfMarks:
            return 0xFE20<= c && c <= 0xFE2F;

          case Category::UnicodeCJKCompatibilityForms:
            return 0xFE30<= c && c <= 0xFE4F;

          case Category::UnicodeSmallFormVariants:
            return 0xFE50<= c && c <= 0xFE6F;

          case Category::UnicodeArabicPresentationFormsB:
            return 0xFE70<= c && c <= 0xFEFE;

          case Category::UnicodeHalfwidthandFullwidthForms:
            return 0xFF00<= c && c <= 0xFFEF;

          case Category::UnicodeSpecials:
            return 0xFEFF <= c && c <= 0xFEFF || 0xFFF0 <= c && c <= 0xFFFD;

            // these block ranges begin above 0x10000

          case Category::UnicodeOldItalic:
          case Category::UnicodeGothic:
          case Category::UnicodeDeseret:
          case Category::UnicodeByzantineMusicalSymbols:
          case Category::UnicodeMusicalSymbols:
          case Category::UnicodeMathematicalAlphanumericSymbols:
          case Category::UnicodeCJKUnifiedIdeographsExtensionB:
          case Category::UnicodeCJKCompatibilityIdeographsSupplement:
          case Category::UnicodeTags:
            return false;

          case Category::LastValue:
          default:
            return false;
          }
        }
      }
    }
  }
