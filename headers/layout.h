#pragma once

#include "widget.h"
#include <stdio.h>
#include <vector>

class Layout {
  friend class Application;

public:
  void setRect(Rect r);
  const Rect getRect() const;
  void setSpaceBetween(unsigned int sb);
  void addWidget(Widget *widget, unsigned int w = 1);
  virtual void updatePosition() { assert(false && "Use VLayout / HLayout"); }

protected:
  unsigned int m_spaceBetween = 0;
  std::vector<Widget *> m_widgets;
  std::vector<unsigned int> m_weights;
  Rect m_rect = {.x = 0, .y = 0, .w = 1, .h = 1};

  const std::vector<Widget *> getWidgets() const { return m_widgets; }
};

class VLayout : public Layout {
public:
  VLayout();
  void updatePosition();
};

class HLayout : public Layout {
public:
  HLayout();
  void updatePosition();
};
