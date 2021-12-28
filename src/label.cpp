#include "../include/label.h"
#include "../include/application.h"
#include <cstdio>

Label::Label() {
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

Label::Label(std::string value) : Label() {
  m_value = value;
}

Label::~Label() {
  XDestroyWindow(m_display, m_window);
  delete m_painter;
}

void Label::paintEvent(XEvent &) {
  m_painter->clear();
  m_painter->setForeground(0x000000);
  m_painter->drawString(m_value.c_str(), 0, m_rect.h / 2);
  m_painter->swapBuffers();
}

void Label::setValue(std::string value) {
  m_value = value;
}
