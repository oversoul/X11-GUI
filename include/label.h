#pragma once
#include "widget.h"
#include <iostream>

class Label : public Widget {
public:
  Label();
  Label(std::string value);
  ~Label();
  void paintEvent() override;
  void setValue(std::string value);

private:
  std::string m_value;
};
