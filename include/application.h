#pragma once
#include <iostream>
#include <vector>

#include "color.h"
#include "font.h"
#include "layout.h"
#include "painter.h"
#include "server.h"
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
  const FontArea getFontArea() const;

  bool isFocused(DrawableId id);

  void setSize(uint, uint);
  void setLayout(Layout &);
  void setLayout(Layout *);

  FontSystem *font();
  void setFont(std::string name, uint size = 12, std::string weight = "normal");

  WindowServer *server() const;
  Display *display() const;
  DrawableId id() const;
  const Painter *painter() const;
  DrawableId window() const;
  DrawableId focusedWindow() const;
  void focusNext(int direction);

  uint width() const;
  uint height() const;

protected:
  void checkForExit();
  void processEvents();

private:
  uint m_width = 0;
  uint m_height = 0;
  uint m_screenWidth = 1;
  uint m_screenHeight = 1;
  int  m_focusNext = 0;

  int m_screen;
  XEvent m_event;
  DrawableId m_window;
  DrawableId m_focusedWindow;

  Atom m_wmDeleteMessage;
  bool m_shouldClose = false;
  Color *m_color = nullptr;
  Layout *m_layout = nullptr;
  FontSystem *m_font = nullptr;

  WindowServer *m_server = nullptr;
  static Application *m_instance;
};
