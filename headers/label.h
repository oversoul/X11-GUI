#pragma once
#include "painter.h"
#include "widget.h"
#include <X11/Xlib.h>
#include <iostream>

class Label : public Widget {
public:
  Label();
  ~Label();
  void paint(XEvent &e);
  void paintEvent(XEvent &);
  void setValue(std::string value);
  const Window id() const { return m_window; }

private:
  std::string m_value;
  Painter *m_painter = nullptr;
};
