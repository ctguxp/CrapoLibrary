#pragma once

namespace System
  {
  namespace Window
    {
    namespace Gui
      {

      class Canvas
        {
        public:
          ~Canvas();
          operator HDC() { return _hdc; }
        protected:
          Canvas(HDC /*hdc*/);
        protected:
          HDC _hdc;
        };

      class PaintCanvas : public Canvas
        {
        public:
          PaintCanvas(HWND /*hwnd*/);
          ~PaintCanvas();
        private:
          HWND        _hwnd;
          PAINTSTRUCT _ps;
        };

      class UpdateCanvas : public Canvas
        {
        public:
          UpdateCanvas(HWND /*hwnd*/);
          ~UpdateCanvas();
        private:
          HWND _hwnd;
        };
      }
    }
  }

