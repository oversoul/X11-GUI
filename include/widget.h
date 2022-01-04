#pragma once
#include "painter.h"
#include "rect.h"
#include "typedefs.h"
#include <assert.h>
#include <iostream>

class Widget {
public:
  Widget();
  ~Widget();

  void newWindow();

  void setRect(Rect r);
  const Window id() const;
  const Rect getRect() const;

  void setBg(ulong c);
  void setWidth(uint w);
  void setHeight(uint h);
  void setPosition(uint x, uint y);

  void updateSizeAndPos();
  bool handleEvent(XEvent &e);
  const bool isFocused() const;

  virtual void paintEvent();
  virtual bool keyPressEvent(KeyEvent);
  virtual bool keyReleaseEvent(KeyEvent);
  virtual bool mousePressEvent(MouseEvent);
  virtual bool mouseScrollEvent(MouseEvent);

protected:
  Window m_window;
  ulong m_bgColor = 0xffffff;
  Rect m_rect = {0, 0, 1, 1};
  bool m_needRepaint = true;
  Display *m_display = nullptr;
  Painter *m_painter = nullptr;
};
