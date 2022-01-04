#pragma once
#include "painter.h"
#include "widget.h"
#include <X11/Xlib.h>
#include <iostream>

class TextInput : public Widget {
public:
  TextInput();
  TextInput(std::string value);
  ~TextInput();
  void paint(XEvent &e);
  void setValue(std::string value);

  void paintEvent(XEvent &) override;
  bool keyPressEvent(KeyEvent) override;
  bool keyReleaseEvent(KeyEvent) override;
  bool mousePressEvent(XButtonEvent &, MouseButton) override;

private:
  std::string m_value;
};
