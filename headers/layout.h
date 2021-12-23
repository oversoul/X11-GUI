#pragma once

#include "widget.h"
#include <stdio.h>
#include <vector>

class Layout {
  friend class Application;

public:
  void setRect(Rect r) { m_rect = r; }
  const Rect getRect() const { return m_rect; }
  void setSpaceBetween(unsigned int sb) { m_spaceBetween = sb; }
  void addWidget(Widget *widget) { m_widgets.push_back(widget); }
  virtual void updatePosition() { assert(false && "Use VLayout / HLayout"); }

protected:
  unsigned int m_spaceBetween = 0;
  const std::vector<Widget *> getWidgets() const { return m_widgets; }
  std::vector<Widget *> m_widgets;
  Rect m_rect = {.x = 0, .y = 0, .w = 1, .h = 1};
};

class VLayout : public Layout {
public:
  VLayout() {}

public:
  void updatePosition() {
    if (m_widgets.empty())
      return;
    if (m_rect.h <= 1)
      return;

    unsigned int totalHeight = m_rect.h - (m_widgets.size() - 1) * m_spaceBetween;

    int ypos = 0;
    int height = totalHeight / m_widgets.size();

    for (auto w_ : m_widgets) {
      w_->setRect({.x = m_rect.x, .y = ypos, .w = m_rect.w, .h = height});
      ypos += height + m_spaceBetween;
    }
  }
};
