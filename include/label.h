#pragma once
#include "painter.h"
#include "widget.h"
#include <X11/Xlib.h>
#include <iostream>

class Label : public Widget {
public:
  Label();
  Label(std::string value);
  ~Label();
  void paintEvent(XEvent &);
  void setValue(std::string value);

private:
  std::string m_value;
};
