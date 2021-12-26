#pragma once

#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>
#include <iostream>

class Application;

class Painter {
public:
  Painter(Display *display, Window window);
  ~Painter();

  void clear();
  void drawString(const char *text, int x, int y);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawPoint(int x, int y);
  void drawRect(int x, int y, int width, int height);
  void fillRect(int x, int y, int width, int height);
  void setForeground(unsigned long color);
  void setBackground(unsigned long color);
  void swapBuffers();

private:
  GC m_gc;
  Window m_window;
  Pixmap m_pixmap;
  XdbeBackBuffer m_backBuffer;
  Display *m_display = nullptr;
  XFontStruct *m_font = nullptr;
};
