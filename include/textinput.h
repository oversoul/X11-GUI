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
  void setValue(std::string value);

  void paintEvent() override;
  bool keyPressEvent(KeyEvent) override;

private:
  std::string m_value;
};
