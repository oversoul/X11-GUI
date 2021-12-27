#include "../headers/button.h"
#include "../headers/application.h"

Button::Button() {
  m_display = Application::instance()->display();
  auto pw = Application::instance()->window();

  // EnterWindowMask | LeaveWindowMask|
  XSetWindowAttributes attr = {
      .background_pixel = m_bgColor,
      .event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask,
  };

  m_window = Widget::createWindow(m_display, {0, 0, 1, 1}, attr, pw);
  m_painter = new Painter(m_display, m_window);
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

bool Button::mousePressEvent(XButtonEvent &e, MouseButton) {
  if (!isFocused())
    return false;
  m_onClickCallback();
  return true;
}

void Button::paintEvent(XEvent &) {
  m_painter->clear();
  m_painter->setForeground(m_bgColor);
  m_painter->fillRect(0, 0, m_rect.w, m_rect.h);
  m_painter->setForeground(0x000000);
  unsigned int width = m_painter->textWidth(m_label.c_str());
  m_painter->drawString(m_label.c_str(), m_rect.w / 2 - width / 2, m_rect.h / 2);
  m_painter->swapBuffers();
}
