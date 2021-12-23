#pragma once
#include "painter.h"
#include "widget.h"
#include <X11/Xlib.h>
#include <iostream>

class TextInput : public Widget {
public:
  TextInput();
  ~TextInput();
  void paint(XEvent &e);
  void setValue(std::string value);

  void paintEvent(XEvent &);
  bool keyPressEvent(XKeyEvent &);
  bool keyReleaseEvent(XKeyEvent &);

  const Window id() const { return m_window; }

private:
  std::string m_value;
  Painter *m_painter = nullptr;
};