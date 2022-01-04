#pragma once
#include "painter.h"
#include "widget.h"
#include <functional>
#include <iostream>

class Button : public Widget {
public:
  Button();
  Button(std::string);
  ~Button();
  void setLabel(std::string);
  void paintEvent() override;
  void setOnClick(std::function<void()>);
  bool mousePressEvent(MouseEvent) override;

private:
  std::string m_label;
  std::function<void()> m_onClickCallback = [] {};
};
