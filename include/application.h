#pragma once
#include <iostream>
#include <vector>

#include "font.h"
#include "layout.h"
#include "painter.h"
#include "widget.h"

class Application {

  friend class HLayout;

public:
  Application(std::string title, bool isModal = false);
  ~Application();
  static Application *instance();

  void setName(char *name);
  void setType(std::string type);
  void setBg(unsigned long color);

  void exec();
  void clear();
  bool shouldExit();
  void triggerExit();
  bool eventPending();

  bool isFocused(Window id);

  void setSize(uint, uint);
  void setLayout(Layout &);
  void setLayout(Layout *);
  void addWidget(Widget *);

  FontSystem *font();
  void setFont(std::string name, uint size = 12);

  Display *display() const;
  const Window id() const;
  const Window window() const;
  const Painter *painter() const;
  const Window focusedWindow() const;

  const uint width() const;
  const uint height() const;

protected:
  void checkForExit();
  void processEvents();

private:
  uint m_width = 0;
  uint m_height = 0;
  uint m_screenWidth = 1;
  uint m_screenHeight = 1;

  int m_root;
  int m_screen;
  XEvent m_event;
  Window m_window;

  Window m_focusedWindow = -1;
  std::vector<Widget *> m_widgets;

  Atom m_wmDeleteMessage;
  bool m_shouldClose = false;
  Layout *m_layout = nullptr;
  FontSystem *m_font = nullptr;
  Display *m_display = nullptr;

  static Application *m_instance;
};
