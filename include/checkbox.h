#pragma once
#include "painter.h"
#include "widget.h"
#include <iostream>

class Checkbox : public Widget {
public:
  Checkbox();
  Checkbox(std::string);
  ~Checkbox();
  void setLabel(std::string);
  void paintEvent() override;
  bool mousePressEvent(MouseEvent) override;

private:
  std::string m_label;
  bool m_isChecked = false;
};
