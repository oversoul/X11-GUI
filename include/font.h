#pragma once
#include <X11/Xft/Xft.h>
#include <iostream>

class FontSystem {
public:
  FontSystem(Display *d, std::string name, uint size);
  ~FontSystem();

  void setFont(std::string, uint);
  XftFont *getFontArea();

private:
  XftFont *m_area;
  Display *m_display;
};
