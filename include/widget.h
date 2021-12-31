#pragma once
#include "rect.h"
#include "painter.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <assert.h>
#include <iostream>

enum class MouseWheelDirection { Up, Down, Unknown };
enum class MouseButton { Unknown, Left, Middle, Right, Scroll };

MouseButton getButton(int btn);
MouseWheelDirection getDirection(int btn);

class Widget {
public:
  virtual const Window id() const;

  Widget();
  ~Widget();


  void newWindow(XSetWindowAttributes attr, bool isChildWindow = true);

  void setRect(Rect r);
  const Rect getRect() const;

  void setWidth(uint w);
  void setHeight(uint h);
  void setPosition(uint x, uint y);

  const bool isFocused() const;
  void updateSizeAndPos();
  bool handleEvent(XEvent &e);

  virtual void paintEvent(XEvent &);
  virtual bool keyPressEvent(KeySym, std::string);
  virtual bool keyReleaseEvent(KeySym, std::string);
  virtual bool mousePressEvent(XButtonEvent &, MouseButton);
  virtual bool mouseScrollEvent(XButtonEvent &, MouseWheelDirection);

protected:
  Window m_window;
  bool m_needRepaint = true;
  Display *m_display = nullptr;
  Painter *m_painter = nullptr;
  unsigned long m_bgColor = 0xffffff;
  Rect m_rect = {.x = 0, .y = 0, .w = 1, .h = 1};
};
