#pragma once
#include <iostream>
#include <vector>

#include "color.h"
#include "font.h"
#include "layout.h"
#include "painter.h"
#include "widget.h"

class Application {

  friend class HLayout;

public:
  Application(std::string name, std::string title = "");
  ~Application();
  static Application *instance();

  void setBg(std::string color);
  void setType(std::string type);

  void exec();
  bool shouldExit();
  void triggerExit();
  bool eventPending();

  bool isFocused(Window id);

  void setSize(uint, uint);
  void setLayout(Layout &);
  void setLayout(Layout *);

  FontSystem *font();
  void setFont(std::string name, uint size = 12, std::string weight = "normal");

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

  int m_screen;
  XEvent m_event;
  Window m_window;
  Window m_focusedWindow = -1;

  Atom m_wmDeleteMessage;
  bool m_shouldClose = false;
  Color *m_color = nullptr;
  Layout *m_layout = nullptr;
  FontSystem *m_font = nullptr;
  Display *m_display = nullptr;

  static Application *m_instance;
};
