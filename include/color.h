#pragma once
#include "../libs/xlib.h"
#include <X11/Xft/Xft.h>
#include <iostream>
#include <map>

class Color {
public:
  Color(Xlib *server);
  ~Color();
  static void set(std::string name);
  static XftColor get(std::string name);

private:
  Xlib *m_server = nullptr;
  static Color *m_instance;
  std::map<std::string, XftColor> m_colors;
};
