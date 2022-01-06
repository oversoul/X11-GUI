#pragma once
#include "../libs/xlib.h"
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
  const Rect getRect() const;
  const DrawableId id() const;

  void setWidth(uint w);
  void setHeight(uint h);
  void setBg(std::string c);
  void setPosition(uint x, uint y);

  void updateSizeAndPos();
  bool handleEvent(Event &e);
  const bool isFocused() const;

  virtual void paintEvent();
  virtual bool keyPressEvent(KeyEvent);
  virtual bool keyReleaseEvent(KeyEvent);
  virtual bool mousePressEvent(MouseEvent);
  virtual bool mouseScrollEvent(MouseEvent);

protected:
  DrawableId m_drawable;
  Rect m_rect = {0, 0, 1, 1};
  Xlib *m_server = nullptr;
  Painter *m_painter = nullptr;

  std::string m_bgColor = "#ffffff";
};
