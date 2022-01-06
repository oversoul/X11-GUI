#include "font.h"
#include "application.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <boost/format.hpp>
#include <cstdio>
#include <string>

FontSystem::FontSystem(Xlib *server, std::string name, uint size, std::string weight) : m_server(server) {
  setFont(name, size, weight);
}

FontSystem::~FontSystem() {
  m_server->closeFontArea();
}

void FontSystem::setFont(std::string name, uint size, std::string weight) {
  std::string fname = (boost::format("%s:pixelsize=%d:weight=%s") % name % size % weight).str();
  m_server->setFontArea(name);
}
