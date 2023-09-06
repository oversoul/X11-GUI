#pragma once
#include "impl.h"
#include "rect.h"
#include <iostream>

class Widget {
public:
  Widget();
  virtual ~Widget();

  void newWindow();

  void setRect(Rect r);
  const DrawableId id() const;
  const Rect getRect() const;

  void setWidth(uint w);
  void setHeight(uint h);
  void setBg(std::string c);
  void setPosition(uint x, uint y);

  bool isFocused() const;
  void updateSizeAndPos();
  virtual bool handleEvent(Event &e);

  virtual void paintEvent();
  virtual void mouseOverEvent();
  virtual bool keyPressEvent(KeyEvent);
  virtual bool keyReleaseEvent(KeyEvent);
  virtual bool mousePressEvent(MouseEvent);
  virtual bool mouseScrollEvent(MouseEvent);

protected:
  DrawableId m_drawable;
  Rect m_rect = {0, 0, 1, 1};
  Painter *m_painter = nullptr;
  WindowServer *m_server = nullptr;

  std::string m_bgColor = "#ffffff";
};
