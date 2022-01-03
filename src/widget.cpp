#include "../include/widget.h"
#include "../include/application.h"
#include "../include/typedefs.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

const Window Widget::id() const {
  return m_window;
}

Widget::Widget() : m_display(Application::instance()->display()) {
}

void Widget::newWindow(XSetWindowAttributes attr, bool isChildWindow) {
  Window p = (isChildWindow) ? Application::instance()->window() : DefaultRootWindow(m_display);
  m_window = createWindow(m_display, {0, 0, 1, 1}, attr, p);
  m_painter = new Painter(m_display, m_window);
}

Widget::~Widget() {
  XDestroyWindow(m_display, m_window);
}

const bool Widget::isFocused() const {
  return Application::instance()->isFocused(id());
}

MouseButton getButton(int btn) {
  if (btn == 1)
    return MouseButton::Left;
  if (btn == 2)
    return MouseButton::Middle;
  if (btn == 3)
    return MouseButton::Right;
  if (btn == 4 || btn == 5)
    return MouseButton::Scroll;
  return MouseButton::Unknown;
}

MouseWheelDirection getDirection(int btn) {
  if (btn == 4)
    return MouseWheelDirection::Up;
  if (btn == 5)
    return MouseWheelDirection::Down;
  return MouseWheelDirection::Unknown;
}

bool Widget::handleEvent(XEvent &e) {
  switch (e.type) {
  case KeyPress:
  case KeyRelease:
    KeySym key;
    char text[255];
    XLookupString(&e.xkey, text, 255, &key, 0);
    if (e.type == KeyPress)
      return keyPressEvent(key, std::string(text));
    if (e.type == KeyRelease)
      return keyReleaseEvent(key, std::string(text));
    return false;
  case ButtonPress:
    if (getButton(e.xbutton.button) == MouseButton::Scroll) {
      return mouseScrollEvent(e.xbutton, getDirection(e.xbutton.button));
    }
    return mousePressEvent(e.xbutton, getButton(e.xbutton.button));
  }
  return false;
};

void Widget::setRect(Rect r) {
  m_rect = r;
}

void Widget::setWidth(uint w) {
  m_rect.w = w;
}

void Widget::setHeight(uint h) {
  m_rect.h = h;
}

void Widget::setPosition(uint x, uint y) {
  m_rect.x = x;
  m_rect.y = y;
}

const Rect Widget::getRect() const {
  return m_rect;
}

void Widget::updateSizeAndPos() {
  XMoveResizeWindow(m_display, m_window, m_rect.x, m_rect.y, m_rect.w, m_rect.h);
}

void Widget::paintEvent(XEvent &) {
}

bool Widget::keyPressEvent(KeySym, std::string) {
  return false;
}

bool Widget::keyReleaseEvent(KeySym, std::string) {
  return false;
}

bool Widget::mousePressEvent(XButtonEvent &, MouseButton) {
  return false;
}

bool Widget::mouseScrollEvent(XButtonEvent &, MouseWheelDirection) {
  return false;
}
