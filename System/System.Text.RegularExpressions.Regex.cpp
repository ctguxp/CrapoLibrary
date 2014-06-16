#include "pch.h"
#include "System.Text.RegularExpressions.Regex.h"
#include "System.Text.RegularExpressions.Syntax.Parser.h"
#include "System.Text.RegularExpressions.Syntax.RegularExpression.h"
#include "System.Text.RegularExpressions.RxCompiler.h"

namespace System
  {
  namespace Text
    {
    namespace RegularExpressions
      {

      FactoryCache Regex::cache(15);      

      Regex::Regex()
        :_groupCount(0)
        ,_gap(0)
        ,_pattern()
        ,_roptions(RegexOptions::None)
        ,_machineFactory(nullptr)
        ,_mapping(nullptr)
        ,_groupNames()
        {
        }

      Regex::Regex(String pattern, RegexOptions options)
        :_groupCount(0)
        ,_gap(0)
        ,_pattern(pattern)
        ,_roptions(options)
        ,_machineFactory(nullptr)
        ,_mapping(nullptr)
        ,_groupNames()
        {
        ValidateOptions(options);
        Init();
        }

      Regex::~Regex()
        {
        }

      void Regex::ValidateOptions(RegexOptions options)
        {
        intptr allopts =
          (intptr)RegexOptions::None |
          (intptr)RegexOptions::IgnoreCase |
          (intptr)RegexOptions::Multiline |
          (intptr)RegexOptions::ExplicitCapture |
          (intptr)RegexOptions::Singleline |
          (intptr)RegexOptions::IgnorePatternWhitespace |
          (intptr)RegexOptions::RightToLeft |
          (intptr)RegexOptions::ECMAScript |
          (intptr)RegexOptions::CultureInvariant;

        intptr ecmaopts =
          (intptr)RegexOptions::IgnoreCase |
          (intptr)RegexOptions::Multiline |
          (intptr)RegexOptions::ECMAScript;

        if(((intptr)options & ~allopts) != 0)
          throw ArgumentOutOfRangeException(L"options");
        if(((intptr)options & (intptr)RegexOptions::ECMAScript) != 0 && ((intptr)options & ~ecmaopts) != 0)
          throw ArgumentOutOfRangeException(L"options");
        }

      void Regex::Init()
        {
        _machineFactory = cache.Lookup(_pattern, _roptions);

        if(_machineFactory == nullptr)
          {
          InitNewRegex();
          } 
        else
          {
          _groupCount = _machineFactory->GroupCount();
          _gap = _machineFactory->Gap();
          _mapping = _machineFactory->Mapping();
          _groupNames = _machineFactory->NamesMapping();
          }
        }

      void Regex::InitNewRegex() 
        {
        _machineFactory = CreateMachineFactory(_pattern, _roptions);
        cache.Add(_pattern, _roptions, _machineFactory);
        _groupCount = _machineFactory->GroupCount();
        _gap = _machineFactory->Gap();
        _mapping = _machineFactory->Mapping();
        _groupNames = _machineFactory->NamesMapping();
        }

      IMachineFactory* Regex::CreateMachineFactory(String pattern, RegexOptions options)
        {
        using namespace Syntax;
        Parser parser;
        RegularExpression* regularExpression = parser.ParseRegularExpression(pattern, options);
        //ICompiler* compiler = nullptr;
        //if(!Regex.old_rx)
         // {
          //if ((options & RegexOptions::Compiled) != RegexOptions::None)
            //{
            //compiler = new CILCompiler();
            //}
          //else
            //{
            //compiler = new RxCompiler();
            //}
          //}
        //else
          //{
          //compiler = new PatternCompiler();
          //}
        //regularExpression.Compile(compiler, (options & RegexOptions.RightToLeft) != RegexOptions.None);
        //IMachineFactory* machineFactory = compiler.GetMachineFactory();
        //Hashtable hashtable = new Hashtable();
        //machineFactory.Gap = parser.GetMapping(hashtable);
        //machineFactory.Mapping = hashtable;
        //machineFactory.NamesMapping = Regex.GetGroupNamesArray(machineFactory.GroupCount, machineFactory.Mapping);
        //return machineFactory;*/
        }

      }
    }
  }
