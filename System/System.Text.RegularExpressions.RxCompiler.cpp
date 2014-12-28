#include "pch.h"
#include "System.Text.RegularExpressions.RxCompiler.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {
      RxLinkRef::RxLinkRef()
        :_offsets(8)
        ,_current(0)
        {
        }
      RxLinkRef::~RxLinkRef()
        {
        }
      void RxLinkRef::PushInstructionBase(int offset)
        {
        if((_current & 1) != 0)
          throw Exception();
        if(_current == (int)_offsets.Length())
          {
          _offsets.Length(_offsets.Length() * 2);
          }
        _offsets[_current++] = offset;
        }
      void RxLinkRef::PushOffsetPosition(int offset)
        {
        if((_current & 1) == 0)
          throw Exception();
        _offsets[_current++] = offset;
        }

      RxInterpreterFactory::RxInterpreterFactory(ByteArray& program, EvalDelegate eval_del)
        :_program(program)
        ,_namesMapping()
        ,_gap(0)
        ,_mapping(nullptr)
        ,_eval_del(eval_del)
        {
        }
      int32 RxInterpreterFactory::GroupCount()
        { 
        return (int32)_program[1] | ((int32)_program[2] << 8);
        }
      StringArray& RxInterpreterFactory::NamesMapping()
        { 
        return _namesMapping;
        }
      void RxInterpreterFactory::NamesMapping(StringArray& value)
        { 
        _namesMapping = value; 
        }
      int32 RxInterpreterFactory::Gap()
        {
        return _gap;
        }
      void RxInterpreterFactory::Gap(int32 value)
        {
        _gap = value;
        }
      Collections::IDictionary* RxInterpreterFactory::Mapping()
        {
        return _mapping;
        }
      void RxInterpreterFactory::Mapping(Collections::IDictionary* value)
        {
        _mapping = value;
        }
      GCIMachine RxInterpreterFactory::NewInstance()
        {
        return GCIMachine(new RxInterpreter(_program, _eval_del));
        }

      RxCompiler::RxCompiler()
        :_curpos(0)
        ,_program(32)
        {
        }
      RxCompiler::~RxCompiler()
        {
        }

      // Overrides
      void RxCompiler::Reset()
        {
        _curpos = 0;
        }
      IMachineFactory* RxCompiler::GetMachineFactory()
        {
        ByteArray dst(_program);
        dst.Length(_curpos);
        return new RxInterpreterFactory(dst, nullptr);
        }
      void RxCompiler::EmitFalse()
        {
        Emit(RxOp::False);
        }
      void RxCompiler::EmitTrue()
        {
        Emit(RxOp::True);
        }
      void RxCompiler::EmitCharacter(wchar_t c, bool negate, bool ignore, bool reverse)
        {
        if(ignore)
          {
          c = Char::ToLower(c);
          }
        if(c < 256)
          {
          EmitOp(RxOp::Char, negate, ignore, reverse);
          Emit((byte)c);
          }
        else
          {
          EmitOp(RxOp::UnicodeChar, negate, ignore, reverse);
          Emit((uint16)c);
          }
        }

      void RxCompiler::EmitCategory(Category cat, bool negate, bool reverse)
        {
        using namespace Globalization;
        switch (cat) 
          {
          case Category::Any:
          case Category::EcmaAny:
            EmitOpNegateReverse(RxOp::CategoryAny, negate, reverse);
            break;
          case Category::AnySingleline:
            EmitOpNegateReverse(RxOp::CategoryAnySingleline, negate, reverse);
            break;
          case Category::Word:
            EmitOpNegateReverse(RxOp::CategoryWord, negate, reverse);
            break;
          case Category::Digit:
            EmitOpNegateReverse (RxOp::CategoryDigit, negate, reverse);
            break;
          case Category::WhiteSpace:
            EmitOpNegateReverse (RxOp::CategoryWhiteSpace, negate, reverse);
            break;
            /* FIXME: translate EcmaWord, EcmaWhiteSpace into Bitmaps? EcmaWhiteSpace will fit very well with the IL engine */
          case Category::EcmaWord:
            EmitOpNegateReverse (RxOp::CategoryEcmaWord, negate, reverse);
            break;
          case Category::EcmaDigit:
            EmitRange ('0', '9', negate, false, reverse);
            break;
          case Category::EcmaWhiteSpace:
            EmitOpNegateReverse (RxOp::CategoryEcmaWhiteSpace, negate, reverse);
            break;
          case Category::UnicodeSpecials:
            EmitOpNegateReverse (RxOp::CategoryUnicodeSpecials, negate, reverse);
            break;
            // Unicode categories...
            // letter
          case Category::UnicodeLu: EmitUniCat (UnicodeCategory::UppercaseLetter, negate, reverse); break;
          case Category::UnicodeLl: EmitUniCat (UnicodeCategory::LowercaseLetter, negate, reverse); break;
          case Category::UnicodeLt: EmitUniCat (UnicodeCategory::TitlecaseLetter, negate, reverse); break;
          case Category::UnicodeLm: EmitUniCat (UnicodeCategory::ModifierLetter, negate, reverse); break;
          case Category::UnicodeLo: EmitUniCat (UnicodeCategory::OtherLetter, negate, reverse); break;
            // mark
          case Category::UnicodeMn: EmitUniCat (UnicodeCategory::NonSpacingMark, negate, reverse); break;
          case Category::UnicodeMe: EmitUniCat (UnicodeCategory::EnclosingMark, negate, reverse); break;
          case Category::UnicodeMc: EmitUniCat (UnicodeCategory::SpacingCombiningMark, negate, reverse); break;
          case Category::UnicodeNd: EmitUniCat (UnicodeCategory::DecimalDigitNumber, negate, reverse); break;
            // number
          case Category::UnicodeNl: EmitUniCat (UnicodeCategory::LetterNumber, negate, reverse); break;
          case Category::UnicodeNo: EmitUniCat (UnicodeCategory::OtherNumber, negate, reverse); break;
            // separator
          case Category::UnicodeZs: EmitUniCat (UnicodeCategory::SpaceSeparator, negate, reverse); break;
          case Category::UnicodeZl: EmitUniCat (UnicodeCategory::LineSeparator, negate, reverse); break;
          case Category::UnicodeZp: EmitUniCat (UnicodeCategory::ParagraphSeparator, negate, reverse); break;
            // punctuation
          case Category::UnicodePd: EmitUniCat (UnicodeCategory::DashPunctuation, negate, reverse); break;
          case Category::UnicodePs: EmitUniCat (UnicodeCategory::OpenPunctuation, negate, reverse); break;
          case Category::UnicodePi: EmitUniCat (UnicodeCategory::InitialQuotePunctuation, negate, reverse); break;
          case Category::UnicodePe: EmitUniCat (UnicodeCategory::ClosePunctuation, negate, reverse); break;
          case Category::UnicodePf: EmitUniCat (UnicodeCategory::FinalQuotePunctuation, negate, reverse); break;
          case Category::UnicodePc: EmitUniCat (UnicodeCategory::ConnectorPunctuation, negate, reverse); break;
          case Category::UnicodePo: EmitUniCat (UnicodeCategory::OtherPunctuation, negate, reverse); break;
            // symbol
          case Category::UnicodeSm: EmitUniCat (UnicodeCategory::MathSymbol, negate, reverse); break;
          case Category::UnicodeSc: EmitUniCat (UnicodeCategory::CurrencySymbol, negate, reverse); break;
          case Category::UnicodeSk: EmitUniCat (UnicodeCategory::ModifierSymbol, negate, reverse); break;
          case Category::UnicodeSo: EmitUniCat (UnicodeCategory::OtherSymbol, negate, reverse); break;
            // other
          case Category::UnicodeCc: EmitUniCat (UnicodeCategory::Control, negate, reverse); break;
          case Category::UnicodeCf: EmitUniCat (UnicodeCategory::Format, negate, reverse); break;
          case Category::UnicodeCo: EmitUniCat (UnicodeCategory::PrivateUse, negate, reverse); break;
          case Category::UnicodeCs: EmitUniCat (UnicodeCategory::Surrogate, negate, reverse); break;
          case Category::UnicodeCn: EmitUniCat (UnicodeCategory::OtherNotAssigned, negate, reverse); break; 
            // Unicode block ranges...
          case Category::UnicodeBasicLatin:
            EmitRange (0x0000, 0x007F, negate, false, reverse); break;
          case Category::UnicodeLatin1Supplement:
            EmitRange (0x0080, 0x00FF, negate, false, reverse); break;
          case Category::UnicodeLatinExtendedA:
            EmitRange (0x0100, 0x017F, negate, false, reverse); break;
          case Category::UnicodeLatinExtendedB:
            EmitRange (0x0180, 0x024F, negate, false, reverse); break;
          case Category::UnicodeIPAExtensions:
            EmitRange (0x0250, 0x02AF, negate, false, reverse); break;
          case Category::UnicodeSpacingModifierLetters:
            EmitRange (0x02B0, 0x02FF, negate, false, reverse); break;
          case Category::UnicodeCombiningDiacriticalMarks:
            EmitRange (0x0300, 0x036F, negate, false, reverse); break;
          case Category::UnicodeGreek:
            EmitRange (0x0370, 0x03FF, negate, false, reverse); break;
          case Category::UnicodeCyrillic:
            EmitRange (0x0400, 0x04FF, negate, false, reverse); break;
          case Category::UnicodeArmenian:
            EmitRange (0x0530, 0x058F, negate, false, reverse); break;
          case Category::UnicodeHebrew:
            EmitRange (0x0590, 0x05FF, negate, false, reverse); break;
          case Category::UnicodeArabic:
            EmitRange (0x0600, 0x06FF, negate, false, reverse); break;
          case Category::UnicodeSyriac:
            EmitRange (0x0700, 0x074F, negate, false, reverse); break;
          case Category::UnicodeThaana:
            EmitRange (0x0780, 0x07BF, negate, false, reverse); break;
          case Category::UnicodeDevanagari:
            EmitRange (0x0900, 0x097F, negate, false, reverse); break;
          case Category::UnicodeBengali:
            EmitRange (0x0980, 0x09FF, negate, false, reverse); break;
          case Category::UnicodeGurmukhi:
            EmitRange (0x0A00, 0x0A7F, negate, false, reverse); break;
          case Category::UnicodeGujarati:
            EmitRange (0x0A80, 0x0AFF, negate, false, reverse); break;
          case Category::UnicodeOriya:
            EmitRange (0x0B00, 0x0B7F, negate, false, reverse); break;
          case Category::UnicodeTamil:
            EmitRange (0x0B80, 0x0BFF, negate, false, reverse); break;
          case Category::UnicodeTelugu:
            EmitRange (0x0C00, 0x0C7F, negate, false, reverse); break;
          case Category::UnicodeKannada:
            EmitRange (0x0C80, 0x0CFF, negate, false, reverse); break;
          case Category::UnicodeMalayalam:
            EmitRange (0x0D00, 0x0D7F, negate, false, reverse); break;
          case Category::UnicodeSinhala:
            EmitRange (0x0D80, 0x0DFF, negate, false, reverse); break;
          case Category::UnicodeThai:
            EmitRange (0x0E00, 0x0E7F, negate, false, reverse); break;
          case Category::UnicodeLao:
            EmitRange (0x0E80, 0x0EFF, negate, false, reverse); break;
          case Category::UnicodeTibetan:
            EmitRange (0x0F00, 0x0FFF, negate, false, reverse); break;
          case Category::UnicodeMyanmar:
            EmitRange (0x1000, 0x109F, negate, false, reverse); break;
          case Category::UnicodeGeorgian:
            EmitRange (0x10A0, 0x10FF, negate, false, reverse); break;
          case Category::UnicodeHangulJamo:
            EmitRange (0x1100, 0x11FF, negate, false, reverse); break;
          case Category::UnicodeEthiopic:
            EmitRange (0x1200, 0x137F, negate, false, reverse); break;
          case Category::UnicodeCherokee:
            EmitRange (0x13A0, 0x13FF, negate, false, reverse); break;
          case Category::UnicodeUnifiedCanadianAboriginalSyllabics:
            EmitRange (0x1400, 0x167F, negate, false, reverse); break;
          case Category::UnicodeOgham:
            EmitRange (0x1680, 0x169F, negate, false, reverse); break;
          case Category::UnicodeRunic:
            EmitRange (0x16A0, 0x16FF, negate, false, reverse); break;
          case Category::UnicodeKhmer:
            EmitRange (0x1780, 0x17FF, negate, false, reverse); break;
          case Category::UnicodeMongolian:
            EmitRange (0x1800, 0x18AF, negate, false, reverse); break;
          case Category::UnicodeLatinExtendedAdditional:
            EmitRange (0x1E00, 0x1EFF, negate, false, reverse); break;
          case Category::UnicodeGreekExtended:
            EmitRange (0x1F00, 0x1FFF, negate, false, reverse); break;
          case Category::UnicodeGeneralPunctuation:
            EmitRange (0x2000, 0x206F, negate, false, reverse); break;
          case Category::UnicodeSuperscriptsandSubscripts:
            EmitRange (0x2070, 0x209F, negate, false, reverse); break;
          case Category::UnicodeCurrencySymbols:
            EmitRange (0x20A0, 0x20CF, negate, false, reverse); break;
          case Category::UnicodeCombiningMarksforSymbols:
            EmitRange (0x20D0, 0x20FF, negate, false, reverse); break;
          case Category::UnicodeLetterlikeSymbols:
            EmitRange (0x2100, 0x214F, negate, false, reverse); break;
          case Category::UnicodeNumberForms:
            EmitRange (0x2150, 0x218F, negate, false, reverse); break;
          case Category::UnicodeArrows:
            EmitRange (0x2190, 0x21FF, negate, false, reverse); break;
          case Category::UnicodeMathematicalOperators:
            EmitRange (0x2200, 0x22FF, negate, false, reverse); break;
          case Category::UnicodeMiscellaneousTechnical:
            EmitRange (0x2300, 0x23FF, negate, false, reverse); break;
          case Category::UnicodeControlPictures:
            EmitRange (0x2400, 0x243F, negate, false, reverse); break;
          case Category::UnicodeOpticalCharacterRecognition:
            EmitRange (0x2440, 0x245F, negate, false, reverse); break;
          case Category::UnicodeEnclosedAlphanumerics:
            EmitRange (0x2460, 0x24FF, negate, false, reverse); break;
          case Category::UnicodeBoxDrawing:
            EmitRange (0x2500, 0x257F, negate, false, reverse); break;
          case Category::UnicodeBlockElements:
            EmitRange (0x2580, 0x259F, negate, false, reverse); break;
          case Category::UnicodeGeometricShapes:
            EmitRange (0x25A0, 0x25FF, negate, false, reverse); break;
          case Category::UnicodeMiscellaneousSymbols:
            EmitRange (0x2600, 0x26FF, negate, false, reverse); break;
          case Category::UnicodeDingbats:
            EmitRange (0x2700, 0x27BF, negate, false, reverse); break;
          case Category::UnicodeBraillePatterns:
            EmitRange (0x2800, 0x28FF, negate, false, reverse); break;
          case Category::UnicodeCJKRadicalsSupplement:
            EmitRange (0x2E80, 0x2EFF, negate, false, reverse); break;
          case Category::UnicodeKangxiRadicals:
            EmitRange (0x2F00, 0x2FDF, negate, false, reverse); break;
          case Category::UnicodeIdeographicDescriptionCharacters:
            EmitRange (0x2FF0, 0x2FFF, negate, false, reverse); break;
          case Category::UnicodeCJKSymbolsandPunctuation:
            EmitRange (0x3000, 0x303F, negate, false, reverse); break;
          case Category::UnicodeHiragana:
            EmitRange (0x3040, 0x309F, negate, false, reverse); break;
          case Category::UnicodeKatakana:
            EmitRange (0x30A0, 0x30FF, negate, false, reverse); break;
          case Category::UnicodeBopomofo:
            EmitRange (0x3100, 0x312F, negate, false, reverse); break;
          case Category::UnicodeHangulCompatibilityJamo:
            EmitRange (0x3130, 0x318F, negate, false, reverse); break;
          case Category::UnicodeKanbun:
            EmitRange (0x3190, 0x319F, negate, false, reverse); break;
          case Category::UnicodeBopomofoExtended:
            EmitRange (0x31A0, 0x31BF, negate, false, reverse); break;
          case Category::UnicodeEnclosedCJKLettersandMonths:
            EmitRange (0x3200, 0x32FF, negate, false, reverse); break;
          case Category::UnicodeCJKCompatibility:
            EmitRange (0x3300, 0x33FF, negate, false, reverse); break;
          case Category::UnicodeCJKUnifiedIdeographsExtensionA:
            EmitRange (0x3400, 0x4DB5, negate, false, reverse); break;
          case Category::UnicodeCJKUnifiedIdeographs:
            EmitRange (0x4E00, 0x9FFF, negate, false, reverse); break;
          case Category::UnicodeYiSyllables:
            EmitRange (0xA000, 0xA48F, negate, false, reverse); break;
          case Category::UnicodeYiRadicals:
            EmitRange (0xA490, 0xA4CF, negate, false, reverse); break;
          case Category::UnicodeHangulSyllables:
            EmitRange (0xAC00, 0xD7A3, negate, false, reverse); break;
          case Category::UnicodeHighSurrogates:
            EmitRange (0xD800, 0xDB7F, negate, false, reverse); break;
          case Category::UnicodeHighPrivateUseSurrogates:
            EmitRange (0xDB80, 0xDBFF, negate, false, reverse); break;
          case Category::UnicodeLowSurrogates:
            EmitRange (0xDC00, 0xDFFF, negate, false, reverse); break;
          case Category::UnicodePrivateUse:
            EmitRange (0xE000, 0xF8FF, negate, false, reverse); break;
          case Category::UnicodeCJKCompatibilityIdeographs:
            EmitRange (0xF900, 0xFAFF, negate, false, reverse); break;
          case Category::UnicodeAlphabeticPresentationForms:
            EmitRange (0xFB00, 0xFB4F, negate, false, reverse); break;
          case Category::UnicodeArabicPresentationFormsA:
            EmitRange (0xFB50, 0xFDFF, negate, false, reverse); break;
          case Category::UnicodeCombiningHalfMarks:
            EmitRange (0xFE20, 0xFE2F, negate, false, reverse); break;
          case Category::UnicodeCJKCompatibilityForms:
            EmitRange (0xFE30, 0xFE4F, negate, false, reverse); break;
          case Category::UnicodeSmallFormVariants:
            EmitRange (0xFE50, 0xFE6F, negate, false, reverse); break;
          case Category::UnicodeArabicPresentationFormsB:
            EmitRange (0xFE70, 0xFEFE, negate, false, reverse); break;
          case Category::UnicodeHalfwidthandFullwidthForms:
            EmitRange (0xFF00, 0xFFEF, negate, false, reverse); break;

            // Complex categories
            /*case Category::UnicodeL:
            case Category::UnicodeM:
            case Category::UnicodeN:
            case Category::UnicodeZ:
            case Category::UnicodeP:
            case Category::UnicodeS:
            case Category::UnicodeC:
            EmitCatGeneral(cat, negate, reverse); break;*/

          default:
            throw NotImplementedException(L"Missing category: "/*+ cat*/);
          }
        }

      void RxCompiler::EmitNotCategory(Category cat, bool negate, bool reverse)
        {
        // not sure why the compiler needed this separate interface funtion
        if (negate) 
          {
          EmitCategory (cat, false, reverse);
          } 
        else
          {
          EmitCategory (cat, true, reverse);
          }
        }

      void RxCompiler::EmitSet(wchar_t lo, Collections::BitArray& set, bool negate, bool ignore, bool reverse)
        {
        int len = (set.Length() + 0x7) >> 3;
        if(lo < 256 && len < 256) 
          {
          EmitOp(RxOp::Bitmap, negate, ignore, reverse);
          Emit((byte)lo);
          Emit((byte)len);
          } 
        else 
          {
          EmitOp(RxOp::UnicodeBitmap, negate, ignore, reverse);
          Emit((uint16)lo);
          Emit((uint16)len);
          }
        // emit the bitmap bytes
        int b = 0;
        while (len-- != 0) 
          {
          int word = 0;
          for (int i = 0; i < 8; ++ i)
            {
            if (b >= set.Length())
              break;
            if(set[b++])
              word |= 1 << i;
            }
          Emit((byte)word);
          }
        }

      void RxCompiler::EmitRange(wchar_t lo, wchar_t hi, bool negate, bool ignore, bool reverse)
        {
        if(lo < 256 && hi < 256)
          {
          EmitOp(RxOp::Range, negate, ignore, reverse);
          Emit((byte)lo);
          Emit((byte)hi);
          }
        else
          {
          EmitOp(RxOp::UnicodeRange, negate, ignore, reverse);
          Emit((uint16)lo);
          Emit((uint16)hi);
          }
        }
      void RxCompiler::EmitString(String str, bool ignore, bool reverse)
        {
        bool flag = false;
        int num = 0;
        if(ignore)
          {
          num++;
          }
        if(reverse)
          {
          num += 2;
          }
        if(ignore)
          {
          str = str.ToLower();
          }
        if(str.Length() < 256)
          {
          flag = true;
          for(int i = 0; i < str.Length(); i++)
            {
            if(str[i] >= 256)
              {
              flag = false;
              break;
              }
            }
          }
        if(flag)
          {
          EmitOpIgnoreReverse(RxOp::String, ignore, reverse);
          Emit((byte)str.Length());
          for(int i = 0; i < str.Length(); i++)
            {
            Emit((byte)str[i]);
            }
          }
        else
          {
          EmitOpIgnoreReverse(RxOp::UnicodeString, ignore, reverse);
          if(str.Length() > 65535)
            {
            //throw NotSupportedException();
            throw SystemException(L"Not Supported");
            }
          Emit((uint16)str.Length());
          for(int i = 0; i < str.Length(); i++)
            {
            Emit((uint16)str[i]);
            }
          }
        }
      void RxCompiler::EmitPosition(Position pos)
        {
        switch (pos)
          {
          case Position::Any:
            Emit(RxOp::AnyPosition);
            break;
          case Position::Start:
            Emit(RxOp::StartOfString);
            break;
          case Position::StartOfString:
            Emit(RxOp::StartOfString);
            break;
          case Position::StartOfLine:
            Emit(RxOp::StartOfLine);
            break;
          case Position::StartOfScan:
            Emit(RxOp::StartOfScan);
            break;
          case Position::End:
            Emit(RxOp::End);
            break;
          case Position::EndOfString:
            Emit(RxOp::EndOfString);
            break;
          case Position::EndOfLine:
            Emit(RxOp::EndOfLine);
            break;
          case Position::Boundary:
            Emit(RxOp::WordBoundary);
            break;
          case Position::NonBoundary:
            Emit(RxOp::NoWordBoundary);
            break;
          default:
            //throw new NotSupportedException ();
            throw SystemException(L"Not Supported");
          }
        }
      void RxCompiler::EmitOpen(int gid)
        {
        if(gid > 65535)
          {
          //throw NotSupportedException();
          throw SystemException(L"Not Supported");
          }
        Emit(RxOp::OpenGroup);
        Emit((uint16)gid);
        }
      void RxCompiler::EmitClose(int gid)
        {
        if(gid > 65535)
          {
          //throw NotSupportedException();
          throw SystemException(L"Not Supported");
          }
        Emit(RxOp::CloseGroup);
        Emit((uint16)gid);
        }
      void RxCompiler::EmitBalanceStart(int gid, int balance, bool capture, LinkRef* tail)
        {
        BeginLink(tail);
        Emit(RxOp::BalanceStart);
        Emit((uint16)gid);
        Emit((uint16)balance);
        Emit((byte)(capture ? 1 : 0));
        EmitLink(tail);
        }
      void RxCompiler::EmitBalance()
        {
        Emit(RxOp::Balance);
        }
      void RxCompiler::EmitReference(int gid, bool ignore, bool reverse)
        {
        if(gid > 65535)
          {
          //throw NotSupportedException();
          throw SystemException(L"Not Supported");
          }
        EmitOpIgnoreReverse(RxOp::Reference, ignore, reverse);
        Emit((uint16)gid);
        }
      void RxCompiler::EmitIfDefined(int gid, LinkRef* tail)
        {
        if(gid > UInt16::MaxValue)
          //throw new NotSupportedException ();
            throw SystemException(L"Not Supported");
        BeginLink(tail);
        Emit(RxOp::IfDefined);
        EmitLink(tail);
        Emit((uint16)gid);
        }
      void RxCompiler::EmitSub(LinkRef* tail)
        {
        BeginLink(tail);
        Emit(RxOp::SubExpression);
        EmitLink(tail);
        }
      void RxCompiler::EmitTest(LinkRef* yes, LinkRef* tail)
        {
        BeginLink(yes);
        BeginLink(tail);
        Emit(RxOp::Test);
        EmitLink(yes);
        EmitLink(tail);
        }
      void RxCompiler::EmitBranch(LinkRef* next)
        {
        BeginLink(next);
        Emit(RxOp::Branch);
        EmitLink(next);
        }
      void RxCompiler::EmitJump(LinkRef* target)
        {
        BeginLink(target);
        Emit(RxOp::Jump);
        EmitLink(target);
        }
      void RxCompiler::EmitRepeat(int min, int max, bool lazy, LinkRef* until)
        {
        BeginLink(until);
        Emit(lazy ? RxOp::RepeatLazy : RxOp::Repeat);
        EmitLink(until);
        Emit(min);
        Emit(max);
        }

      void RxCompiler::EmitUntil(LinkRef* repeat)
        {
        ResolveLink(repeat);
        Emit(RxOp::Until);
        }

      void RxCompiler::EmitInfo(int32 count, int32 min, int32 max)
        {
        Emit(RxOp::Info);
        if(count > 65535)
          {
          //throw NotSupportedException();
          throw SystemException(L"Not Supported");
          }
        Emit((uint16)count);
        Emit(min);
        Emit(max);
        }

      void RxCompiler::EmitFastRepeat(int min, int max, bool lazy, LinkRef* tail)
        {
        BeginLink(tail);
        Emit(lazy ? RxOp::FastRepeatLazy : RxOp::FastRepeat);
        EmitLink(tail);
        Emit(min);
        Emit(max);
        }

      void RxCompiler::EmitIn(LinkRef* tail)
        {
        // emitted for things like [\dabcfh]
        BeginLink (tail);
        Emit(RxOp::TestCharGroup);
        EmitLink(tail);
        }

      void RxCompiler::EmitAnchor(bool reverse, int offset, LinkRef* tail)
        {
        BeginLink(tail);
        if(reverse)
          Emit(RxOp::AnchorReverse);
        else
          Emit(RxOp::Anchor);
        EmitLink(tail);
        if (offset > UInt16::MaxValue)
          //throw new NotSupportedException ();
            throw SystemException(L"Not Supported");
        Emit((uint16)offset);
        }
      /*void RxCompiler::EmitBranchEnd()
      {
      }
      void RxCompiler::EmitAlternationEnd()
      {
      }*/
      // Virtual functions
      void RxCompiler::EmitOp(RxOp op, bool negate, bool ignore, bool reverse)
        {
        int num = 0;
        if(negate)
          {
          num++;
          }
        if(ignore)
          {
          num += 2;
          }
        if(reverse)
          {
          num += 4;
          }
        Emit((byte)((byte)op + (byte)num));
        }

      void RxCompiler::EmitOpIgnoreReverse(RxOp op, bool ignore, bool reverse)
        {
        int32 num = 0;
        if(ignore)
          {
          num++;
          }
        if(reverse)
          {
          num += 2;
          }
        Emit((byte)((byte)op + (byte)num));
        }

      void RxCompiler::EmitOpNegateReverse (RxOp op, bool negate, bool reverse) 
        {
        int offset = 0;
        if(negate)
          offset += 1;
        if(reverse)
          offset += 2;

        Emit((RxOp)((int32)op + offset));
        }

      LinkRef* RxCompiler::NewLink()
        {
        return new RxLinkRef();
        }
      void RxCompiler::ResolveLink(LinkRef* link)
        {
        RxLinkRef& l = static_cast<RxLinkRef&>(*link);
        for (int i = 0; i < l._current; i += 2)
          {
          int offset = _curpos - l._offsets[i];
          if (offset > UInt16::MaxValue)
            //throw new NotSupportedException();
              throw SystemException(L"Not Supported");
          int offsetpos = l._offsets [i + 1];
          _program[offsetpos] = (byte)offset;
          _program[offsetpos + 1] = (byte)(offset >> 8);
          }
        }

      // Private functions
      void RxCompiler::BeginLink(LinkRef* lref)
        {
        RxLinkRef& link = static_cast<RxLinkRef&>(*lref);
        link.PushInstructionBase(_curpos);
        }
      void RxCompiler::EmitLink(LinkRef* lref)
        {
        RxLinkRef& link = static_cast<RxLinkRef&>(*lref);
        link.PushOffsetPosition(_curpos);
        Emit((uint16)0);
        }
      void RxCompiler::Emit(byte val)
        {
        MakeRoom(1);
        _program[_curpos] = val;
        _curpos++;
        }
      void RxCompiler::Emit(uint16 val)
        {
        MakeRoom(2);
        _program[_curpos] = (byte)val;
        _program[_curpos + 1] = (byte)(val >> 8);
        _curpos += 2;
        }
      void RxCompiler::Emit(int32 val)
        {
        MakeRoom(4);
        _program[_curpos] = (byte)val;
        _program[_curpos + 1] = (byte)(val >> 8);
        _program[_curpos + 2] = (byte)(val >> 16);
        _program[_curpos + 3] = (byte)(val >> 24);
        _curpos += 4;
        }
      void RxCompiler::Emit(RxOp opcode)
        {
        Emit((byte)opcode);
        }
      void RxCompiler::EmitUniCat(Globalization::UnicodeCategory cat, bool negate, bool reverse)
        {
        EmitOpNegateReverse(RxOp::CategoryUnicode, negate, reverse);
        Emit ((byte)cat);
        }
      void RxCompiler::MakeRoom(int bytes)
        {
        while(_curpos + bytes > (int)_program.Length())
          {
          _program.Length(_program.Length() * 2);
          }
        }
      }
    }
  }
