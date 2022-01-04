#include "../include/button.h"
#include "../include/application.h"
#include "../include/typedefs.h"

Button::Button() {
  XSetWindowAttributes attr = {
      .background_pixel = m_bgColor,
      .event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask,
  };

  newWindow(attr);
}

Button::Button(std::string label) : Button() {
  setLabel(label);
}

Button::~Button() {
}

void Button::setLabel(std::string label) {
  m_label = label;
}

void Button::setOnClick(std::function<void()> callback) {
  m_onClickCallback = callback;
}

bool Button::mousePressEvent(MouseEvent e) {
  if (!isFocused() && e.button != MouseButton::Left)
    return false;
  m_onClickCallback();
  return true;
}

void Button::paintEvent() {
  m_painter->clear(m_bgColor);
  uint width = m_painter->textWidth(m_label.c_str());
  m_painter->drawString(m_label.c_str(), m_rect.w / 2 - width / 2, m_rect.h / 2);
  m_painter->swapBuffers();
}
