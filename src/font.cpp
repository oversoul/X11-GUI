#include "font.h"
#include "application.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <string>

FontSystem::FontSystem(Display *d, std::string name, uint size, std::string weight) : m_display(d) {
  setFont(name, size, weight);
}

FontSystem::~FontSystem() {
  XftFontClose(m_display, m_area);
  // delete m_area;
}

void FontSystem::setFont(std::string name, uint size, std::string weight) {
  if (m_area != nullptr)
    XftFontClose(m_display, m_area);

  int s = DefaultScreen(m_display);

  m_area = XftFontOpenName(m_display, s, (name + ":pixelsize=" + std::to_string(size) + ":weight=" + weight).c_str());

  if (!m_area) {
    fprintf(stderr, "Couldn't open font.\n");
    exit(1);
  }
}

XftFont *FontSystem::getFontArea() {
  return m_area;
}
