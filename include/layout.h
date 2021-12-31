#pragma once

#include "widget.h"
#include <stdio.h>
#include <vector>

class Layout {
  friend class Application;

public:
  void setRect(Rect r);
  const Rect getRect() const;
  void setSpaceBetween(uint);
  void setFullSize(uint = 0, uint = 0);
  const Widget *getFirstWidget() const;
  void addWidget(Widget *widget, uint w = 1);

  virtual void updatePosition() {
    assert(false && "Use VLayout / HLayout");
  }

protected:
  Rect m_rect = {0, 0, 1, 1};
  uint m_spaceBetween = 0;
  std::vector<uint> m_weights;
  std::vector<Widget *> m_widgets;

  const std::vector<Widget *> getWidgets() const {
    return m_widgets;
  }
};

class VLayout : public Layout {
public:
  VLayout();
  ~VLayout();
  void updatePosition();
};

class HLayout : public Layout {
public:
  HLayout();
  ~HLayout();
  void updatePosition();
};
