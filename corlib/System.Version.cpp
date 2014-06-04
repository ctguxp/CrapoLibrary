#include "pch.h"
#include "System.Version.h"
#include "System.Exception.h"
#include "System.Int32.h"

namespace System
  {
  Version::Version(int major, int minor)
    :_major(major)
    ,_minor(minor)
    ,_build(-1)
    ,_revision(-1)
    {
    }
  Version::Version(int major, int minor, int build)
    :_major(major)
    ,_minor(minor)
    ,_build(build)
    ,_revision(-1)
    {
    }
  Version::Version(int major, int minor, int build, int revision)
    :_major(major)
    ,_minor(minor)
    ,_build(build)
    ,_revision(revision)
    {
    }
  Version::Version(String version)
    :_major(0)
    ,_minor(0)
    ,_build(-1)
    ,_revision(-1)
    {
    int major = -1, minor = -1, build = -1, revision = -1;

    CharArray delim(1);
    delim[0] = L'.';
    StringArray vals = version.Split(delim);
    sizet n = vals.Length();

    if(n < 2 || n > 4)
      {
      throw ArgumentException(L"There must be 2, 3 or 4 components in the version string.");
      }

    if(n > 0)
      major = Int32::Parse(vals[0]);
    if(n > 1)
      minor = Int32::Parse(vals[1]);
    if(n > 2)
      build = Int32::Parse(vals[2]);
    if(n > 3)
      revision = Int32::Parse(vals[3]);

    CheckedSet(n, major, minor, build, revision);
    }
  Version::Version(const Version& ver)
    :_major(ver._major)
    ,_minor(ver._minor)
    ,_build(ver._build)
    ,_revision(ver._revision)
    {
    }    
  Version::~Version()
    {
    }
  Version& Version::operator=(const Version& ver)
    {
    if(this == &ver)
      return *this;

    _major = ver._major;
    _minor = ver._minor;
    _build = ver._build;
    _revision = ver._revision;

    return *this;
    }
  void Version::CheckedSet(sizet defined, int major, int minor, int build, int revision)
    {
    // defined should be 2, 3 or 4

    if(major < 0)
      {
      throw ArgumentOutOfRangeException(L"major");
      }
    _major = major;

    if(minor < 0) 
      {
      throw ArgumentOutOfRangeException(L"minor");
      }
    _minor = minor;

    if(defined == 2)
      {
      _build = UNDEFINED;
      _revision = UNDEFINED;
      return;
      }

    if(build < 0)
      {
      throw ArgumentOutOfRangeException(L"build");
      }
    _build = build;

    if(defined == 3)
      {
      _revision = UNDEFINED;
      return;
      }

    if (revision < 0) 
      {
      throw ArgumentOutOfRangeException(L"revision");
      }
    _revision = revision;
    }
  }
