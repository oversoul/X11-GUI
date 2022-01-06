#pragma once
#include "../libs/xlib.h"
#include <iostream>

class FontSystem {
public:
  FontSystem(WindowServer *server, std::string name, uint size, std::string weight = "normal");
  ~FontSystem();

  void setFont(std::string, uint, std::string = "normal");

private:
  WindowServer *m_server = nullptr;
};
