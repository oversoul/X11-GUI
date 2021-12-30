#pragma once
#include <iostream>
#include <vector>

#include "layout.h"
#include "painter.h"
#include "widget.h"

class Application {

  friend class HLayout;

public:
  Application(std::string title, bool isModal = false);
  ~Application();
  static Application *instance();
  void setType(std::string type);

  void setBackground(unsigned long color);

  void exec();
  void clear();
  bool shouldExit();
  void triggerExit();
  bool eventPending();

  bool isFocused(Window id);

  void setSize(unsigned int, unsigned int);
  void setLayout(Layout &);
  void setLayout(Layout *);
  void addWidget(Widget *);

  Display *display() const;
  const Window id() const;
  const Window window() const;
  const Painter *painter() const;
  const Window focusedWindow() const;

  const unsigned int width() const;
  const unsigned int height() const;

protected:
  void checkForExit();
  void processEvents();

private:
  unsigned int m_width = 0;
  unsigned int m_height = 0;
  unsigned int m_screenWidth = 1;
  unsigned int m_screenHeight = 1;

  int m_root;
  int m_screen;
  XEvent m_event;
  Window m_window;

  Window m_focusedWindow = -1;
  std::vector<Widget *> m_widgets;

  Atom m_wmDeleteMessage;
  bool m_shouldClose = false;
  Layout *m_layout = nullptr;
  Display *m_display = nullptr;

  static Application *m_instance;
};
