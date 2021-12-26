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
  Application(std::string title, unsigned int width, unsigned int height);
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

  Display *display() const;
  const Window id() const;
  const Window window() const;
  const Painter *painter() const;
  const Window focusedWindow() const;

  const int width() const;
  const int height() const;

protected:
  void checkForExit();
  void processEvents();

private:
  unsigned int m_width = 0;
  unsigned int m_height = 0;

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
