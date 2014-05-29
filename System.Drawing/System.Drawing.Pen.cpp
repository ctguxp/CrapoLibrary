#include "pch.h"
#include "System.Drawing.Pen.h"
#include "System.Drawing.GDIPlus.h"
#include "System.Drawing.GraphicsUnit.h"

namespace System
  {
  namespace Drawing
    {
    Pen::Pen(Color color, float width)
      :_color(color)
      ,_nativeObject(nullptr)
      {
      void* out;
      int status = GDIPlus::GdipCreatePen1(color.ToArgb(), width, (int)GraphicsUnit::World, &out);
      //GDIPlus.CheckStatus(status);
      _nativeObject = out;
      }
    Pen::~Pen()
      {
      if(_nativeObject != nullptr)
        {
        int status = GDIPlus::GdipDeletePen(_nativeObject);
        _nativeObject = nullptr;
        //GDIPlus.CheckStatus(status);
        }
      }
    }
  }
