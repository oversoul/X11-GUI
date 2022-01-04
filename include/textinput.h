#pragma once
#include "widget.h"
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
