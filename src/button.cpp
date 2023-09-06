#include "button.h"
#include "typedefs.h"
#include "application.h"

Button::Button() {
  newWindow();
}

Button::Button(std::string label) : Button() {
  setLabel(label);
}

Button::~Button() {}

void Button::setLabel(std::string label) {
  m_label = label;
}

void Button::setOnClick(std::function<void()> callback) {
  m_onClickCallback = callback;
}

void Button::mouseOverEvent() {
  Application::instance()->setCursor(CursorType::Hand);
}

bool Button::mousePressEvent(MouseEvent e) {
  if (!isFocused() || e.button != MouseButton::Left)
    return false;

  m_onClickCallback();
  return true;
}

void Button::paintEvent() {
  m_painter->clear(m_bgColor, m_rect);
  m_painter->setForeground(isFocused() ? "#ff0000" : "#000000");
  uint width = m_painter->textWidth(m_label.c_str());
  m_painter->drawString(m_label.c_str(), m_rect.w / 2 - width / 2, m_rect.h / 2);
  m_painter->roundedRect({0, 0, m_rect.w - 1, m_rect.h - 1}, 10);
  m_painter->swapBuffers();
}
