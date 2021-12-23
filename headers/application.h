#pragma once
#include <X11/Xlib.h>
#include <iostream>
#include <vector>

#include "painter.h"
#include "widget.h"

class Application : public Widget {

public:
  Application(std::string title, int width, int height);
  ~Application();
  static Application *instance();

  void exit();
  void exec();
  void clear();
  void redraw();
  bool shouldExit();
  bool eventPending();

  void setInputSelection(long types = ButtonPressMask | ButtonReleaseMask | ExposureMask | EnterWindowMask |
                                      LeaveWindowMask | KeyPressMask);

  void addWidget(Widget *);
  long getInputSelection();
  Painter *painter() { return m_painter; }
  Window window() const { return m_window; }
  Display *display() const { return m_display; }

private:
  int m_root;
  int m_screen;
  XEvent m_event;
  Window m_window;

  std::vector<Widget *> m_widgets;

  long m_input_types;
  Atom m_wmDeleteMessage;
  bool m_shouldClose = false;
  Painter *m_painter = nullptr;
  Display *m_display = nullptr;

  static Application *m_instance;
};
