#pragma once
#include <X11/Xlib.h>
#include <iostream>

#include "painter.h"

class Application {
public:
  Application(std::string title, int width, int height);
  ~Application();

  void exit();
  void clear();
  void redraw();
  XEvent fetchEvent();
  void setInputSelection(long types = ExposureMask | ButtonPressMask | KeyPressMask);
  bool shouldExit();

  void setBackground(unsigned long color);
  void setForeground(unsigned long color);

  GC gc() const { return m_gc; }
  Window window() const { return m_window; }
  Display *display() const { return m_display; }

  Painter *painter() { return m_painter; }

private:
  GC m_gc;
  int m_root;
  int m_screen;
  XEvent m_event;
  Window m_window;
  Atom m_wmDeleteMessage;
  bool m_shouldClose = false;
  Painter *m_painter = nullptr;
  Display *m_display = nullptr;
};
