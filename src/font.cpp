#include <string>
#include "font.h"
#include "application.h"

FontSystem::FontSystem(WindowServer *server, std::string name, uint size, std::string weight) : m_server(server) {
  setFont(name, size, weight);
}

FontSystem::~FontSystem() {
  m_server->closeFontArea();
}

void FontSystem::setFont(std::string name, uint size, std::string weight) {
  char fname[100];
  std::snprintf(fname, 100, "%s:pixelsize=%d:width=%s", name.c_str(), size, weight.c_str());
  m_server->setFontArea(fname);
}
