#include "font.h"
#include "application.h"
#include <boost/format.hpp>
#include <string>

FontSystem::FontSystem(WindowServer *server, std::string name, uint size, std::string weight) : m_server(server) {
  setFont(name, size, weight);
}

FontSystem::~FontSystem() {
  m_server->closeFontArea();
}

void FontSystem::setFont(std::string name, uint size, std::string weight) {
  std::string fname = (boost::format("%s:pixelsize=%d:weight=%s") % name % size % weight).str();
  m_server->setFontArea(fname);
}
