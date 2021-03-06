#include "../include/label.h"
#include "../include/application.h"
#include "../include/typedefs.h"
#include <cstdio>

Label::Label() {
  newWindow();
}

Label::Label(std::string value) : Label() {
  m_value = value;
}

Label::~Label() {
}

void Label::paintEvent() {
  m_painter->clear(m_bgColor, m_rect);
  m_painter->drawString(m_value.c_str(), 0, m_rect.h / 2);
  m_painter->swapBuffers();
}

void Label::setValue(std::string value) {
  m_value = value;
}
