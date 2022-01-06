#include "color.h"
#include "application.h"
#include <stdexcept>

Color *Color::m_instance = nullptr;

Color::Color(Xlib *server) : m_server(server) {
  if (m_instance)
    throw std::runtime_error("Color already initialized.");
  m_instance = this;
  Color::set("#000000");
  // set("#FFFFFF");
}

void Color::set(std::string name) {
  if (!m_instance)
    throw new std::runtime_error("Color needs to be initialized first");

  if (m_instance->m_colors.count(name))
    return;

  if (m_instance->m_server->setColor(&m_instance->m_colors[name], name) < 0)
    throw std::runtime_error("Couldn't allocate color");
}

Color::~Color() {
  m_colors.clear();
  delete m_instance;
}

XftColor Color::get(std::string name) {
  if (!m_instance)
    throw new std::runtime_error("Color needs to be initialized first");

  assert(name.length() == 7 && "Color name is empty\n");

  if (!m_instance->m_colors.count(name))
    set(name);
  return m_instance->m_colors[name];
}
