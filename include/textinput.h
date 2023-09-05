#pragma once
#include "widget.h"
#include "application.h"
#include <iostream>

class TextInput : public Widget {
public:
  TextInput();
  TextInput(std::string value);
  ~TextInput();
  void setValue(std::string value);
  void setPadding(uint padding);

  void paintEvent() override;
  void mouseOverEvent(bool) override;
  bool keyPressEvent(KeyEvent) override;
  bool mousePressEvent(MouseEvent) override;

  bool handleHotKeys(KeyEvent);

private:
  uint m_cursor = 0;
  uint m_padding = 10;
  std::string m_value;
};
