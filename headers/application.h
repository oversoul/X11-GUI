#pragma once
#include <X11/Xlib.h>
#include <iostream>
#include <vector>

#include "layout.h"
#include "painter.h"
#include "widget.h"

class Application {

  friend class HLayout;

public:
  Application(std::string title, int width, int height);
  ~Application();
  static Application *instance();

  void exit();
  void exec();
  void clear();
  bool shouldExit();
  bool eventPending();

  bool isFocused(Window id);

  void setLayout(Layout &);
  void setLayout(Layout *);
  void addWidget(Widget *);
  Painter *painter() { return m_painter; }
  const Window window() const { return m_window; }
  const Window id() const { return m_window; }
  Display *display() const { return m_display; }
  const Window focusedWindow() const { return m_focusedWindow; }

  const int width() const;
  const int height() const;

protected:
  void checkForExit();
  void processEvents();

private:
  int m_width = 0;
  int m_height = 0;

  int m_root;
  int m_screen;
  XEvent m_event;
  Window m_window;

  Window m_focusedWindow = -1;
  std::vector<Widget *> m_widgets;

  Atom m_wmDeleteMessage;
  bool m_shouldClose = false;
  Layout *m_layout = nullptr;
  Painter *m_painter = nullptr;
  Display *m_display = nullptr;

  static Application *m_instance;
};
