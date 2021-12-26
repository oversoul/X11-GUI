#pragma once
#include "painter.h"
#include "widget.h"
#include <X11/Xlib.h>
#include <functional>
#include <iostream>

class Button : public Widget {
public:
  Button();
  ~Button();
  void setLabel(std::string);
  void paintEvent(XEvent &);
  void setOnClick(std::function<void()>);
  bool mousePressEvent(XButtonEvent &, MouseButton);

private:
  std::string m_label;
  Painter *m_painter = nullptr;
  unsigned long m_bgColor = 0xCCCCCC;
  std::function<void()> m_onClickCallback = [] {};
};
