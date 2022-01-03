#pragma once
#include "painter.h"
#include "rect.h"
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

  void setBg(ulong c);
  void setWidth(uint w);
  void setHeight(uint h);
  void setPosition(uint x, uint y);

  void updateSizeAndPos();
  bool handleEvent(XEvent &e);
  const bool isFocused() const;

  virtual void paintEvent(XEvent &);
  virtual bool keyPressEvent(KeySym, std::string);
  virtual bool keyReleaseEvent(KeySym, std::string);
  virtual bool mousePressEvent(XButtonEvent &, MouseButton);
  virtual bool mouseScrollEvent(XButtonEvent &, MouseWheelDirection);

protected:
  Window m_window;
  Rect m_rect = {0, 0, 1, 1};
  bool m_needRepaint = true;
  Display *m_display = nullptr;
  Painter *m_painter = nullptr;
  unsigned long m_bgColor = 0xffffff;
};
