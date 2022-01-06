#pragma once
#include "../libs/xlib.h"
#include <X11/Xft/Xft.h>
#include <iostream>
#include <map>

class Color {
public:
  Color(WindowServer  *server);
  ~Color();
  static void set(std::string name);
  static XftColor get(std::string name);

private:
  WindowServer *m_server = nullptr;
  static Color *m_instance;
  std::map<std::string, XftColor> m_colors;
};
