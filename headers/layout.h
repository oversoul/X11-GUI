#pragma once

#include "widget.h"
#include <stdio.h>
#include <vector>

class Layout {
  friend class Application;

public:
  void setRect(Rect r);
  void setFullSize(unsigned int = 0);
  const Rect getRect() const;
  void setSpaceBetween(unsigned int sb);
  void addWidget(Widget *widget, unsigned int w = 1);

  virtual void updatePosition() {
    assert(false && "Use VLayout / HLayout");
  }

protected:
  Rect m_rect = {0, 0, 1, 1};
  unsigned int m_spaceBetween = 0;
  std::vector<Widget *> m_widgets;
  std::vector<unsigned int> m_weights;

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
