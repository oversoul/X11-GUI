#pragma once
#include "../libs/xlib.h"
#include <iostream>
#include <map>

class Color {
public:
  Color(WindowServer *server);
  ~Color();
  static void set(std::string name);
  static IColor get(std::string name);

private:
  static Color *m_instance;
  WindowServer *m_server = nullptr;
  std::map<std::string, IColor> m_colors;
};
