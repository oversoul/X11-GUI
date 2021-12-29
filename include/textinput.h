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

  void paintEvent(XEvent &);
  bool keyPressEvent(KeySym, std::string);
  bool keyReleaseEvent(KeySym, std::string);
  bool mousePressEvent(XButtonEvent &, MouseButton);

private:
  std::string m_value;
};
