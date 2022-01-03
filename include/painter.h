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

  void initializeColor(std::string color);

  void clear(unsigned long = 0xFFFFFF);
  void drawPoint(int x, int y);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawRect(Rect r);
  void fillRect(Rect r);
  void drawCircle(Rect r);
  void fillCircle(Rect r);
  void drawString(const char *text, int x, int y, std::string color = "#000000");
  void setForeground(unsigned long color);
  void setBackground(unsigned long color);
  uint textWidth(const char *text);
  void swapBuffers();

private:
  GC m_gc;
  Window m_window;
  Pixmap m_pixmap;
  XdbeBackBuffer m_backBuffer;
  Display *m_display = nullptr;
  FontSystem *m_font = nullptr;

  XftColor m_color;
  XftDraw *m_draw;
  std::map<std::string, XftColor> m_colors;
};
