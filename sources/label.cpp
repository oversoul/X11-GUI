#include "../headers/label.h"
#include "../headers/application.h"
#include "../headers/widget.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdio>

Label::Label() {
  m_display = Application::instance()->display();
  auto pw = Application::instance()->window();

  Rect r = {0, 0, 1, 1};

  XSetWindowAttributes attr = {
      .background_pixel = m_bgColor,
      .event_mask = ExposureMask | EnterWindowMask | LeaveWindowMask,
  };

  m_window = Widget::createWindow(m_display, r, attr.event_mask, pw);

  m_painter = new Painter(m_display, m_window);
  m_painter->clear();
}

Label::~Label() {
  //
}

void Label::paintEvent(XEvent &) {
  m_painter->clear();
  setBackground(m_bgColor);

  m_painter->setForeground(0x000000);
  m_painter->drawString(m_value.c_str(), 5, m_rect.h / 2);
}

void Label::setValue(std::string value) { //
  m_value = value;
}
