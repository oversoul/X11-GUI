#pragma once
#include "xcb.h"
#include "font.h"
#include "painter.h"
#include "rect.h"
#include <iostream>

class XcbPainter : public Painter {
public:
  XcbPainter(xcb_connection_t *conn, xcb_screen_t *screen, DrawableId window, FontArea ft);
  ~XcbPainter();

  void swapBuffers() override;
  void drawRect(Rect r) override;
  void fillRect(Rect r) override;
  void drawCircle(Rect r) override;
  void fillCircle(Rect r) override;
  void drawPoint(int x, int y) override;
  uint textWidth(const char *text) override;
  void setForeground(std::string color) override;
  void clear(std::string color, Rect r) override;
  void drawLine(int x1, int y1, int x2, int y2) override;
  void drawString(const char *text, int x, int y, std::string color = "#000000") override;

private:
  xcb_gcontext_t m_gc;
  FontSystem *m_font = nullptr;
  xcb_connection_t *m_connection = nullptr;
  xcb_screen_t *m_screen;
  DrawableId m_window;
  FontArea m_area;
};
