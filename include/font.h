#pragma once
#include "../libs/xlib.h"
#include <iostream>

class FontSystem {
public:
  FontSystem(Xlib *server, std::string name, uint size, std::string weight = "normal");
  ~FontSystem();

  void setFont(std::string, uint, std::string = "normal");

private:
  Xlib *m_server = nullptr;
};
