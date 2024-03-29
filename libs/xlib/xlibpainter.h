#pragma once
#include "font.h"
#include "rect.h"
#include "painter.h"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xdbe.h>

class XlibPainter : public Painter {
public:
  XlibPainter(Display *display, Window window, FontArea area);
  ~XlibPainter();

  void swapBuffers() override;
  void drawRect(Rect r) override;
  void fillRect(Rect r) override;
  void drawCircle(Rect r) override;
  void fillCircle(Rect r) override;
  void drawPoint(int x, int y) override;
  uint textWidth(const char *text) override;
  void setForeground(std::string color) override;
  void clear(std::string color, Rect r) override;
  void drawLine(int x1, int y1, int x2, int y2, int w = 1) override;
  void drawString(const char *text, int x, int y, std::string color = "#000000") override;
  void roundedRect(Rect r, uint radius) override;

private:
  GC m_gc;
  XftDraw *m_draw;
  XdbeBackBuffer m_backBuffer;
  Display *m_display = nullptr;
  DrawableId m_window;
  FontArea m_area;
  FontSystem *m_font = nullptr;
};
