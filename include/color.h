#pragma once
#include <X11/Xft/Xft.h>
#include <iostream>
#include <map>

class Color {
public:
  Color(Display* dpy);
  ~Color();
  static void set(std::string name);
  static XftColor get(std::string name);

private:
  static Color *m_instance;

  Display *m_display;
  std::map<std::string, XftColor> m_colors;
};
