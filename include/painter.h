#pragma once
#include "font.h"
#include "rect.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>
#include <iostream>
#include <map>

class Application;

class Painter {
public:
  Painter(Display *display, Window window);
  ~Painter();

  void drawPoint(int x, int y);
  void drawRect(Rect r);
  void drawLine(int x1, int y1, int x2, int y2);
  void fillRect(Rect r);
  void drawCircle(Rect r);
  void fillCircle(Rect r);
  uint textWidth(const char *text);
  void drawString(const char *text, int x, int y, std::string color = "#000000");
  void setForeground(std::string color);
  void clear(std::string color, Rect r);
  void swapBuffers();

private:
  GC m_gc;
  Window m_window;
  XftDraw *m_draw;
  XdbeBackBuffer m_backBuffer;
  Display *m_display = nullptr;
  FontSystem *m_font = nullptr;
};
