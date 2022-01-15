#include "checkbox.h"
#include "application.h"
#include "typedefs.h"

Checkbox::Checkbox() {
  newWindow();
}

Checkbox::Checkbox(std::string label) : Checkbox() {
  setLabel(label);
}

Checkbox::~Checkbox() {
}

void Checkbox::setLabel(std::string label) {
  m_label = label;
}

bool Checkbox::mousePressEvent(MouseEvent e) {
  if (!isFocused() || e.button != MouseButton::Left)
    return false;

  m_isChecked = !m_isChecked;
  return true;
}

void Checkbox::paintEvent() {
  m_painter->clear(m_bgColor, m_rect);

  uint pd = 4;
  uint size = 20;
  uint padd = (m_rect.h - size) / 2;

  m_painter->setForeground("#333333");
  m_painter->drawRect({0, padd, size, size});

  if (m_isChecked)
    m_painter->fillRect({pd, padd + pd, size - 2 * pd, size - 2 * pd});

  m_painter->drawString(m_label.c_str(), 30, m_rect.h / 2);
  m_painter->swapBuffers();
}
