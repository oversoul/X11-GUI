#include "../include/widget.h"
#include "../include/application.h"
#include "../include/typedefs.h"

Widget::Widget() : m_display(Application::instance()->display()) {
}

const Window Widget::id() const {
  return m_window;
}

void Widget::newWindow(XSetWindowAttributes attr, bool isChildWindow) {
  Window p = (isChildWindow) ? Application::instance()->window() : DefaultRootWindow(m_display);
  m_window = createWindow(m_display, attr, p);
  m_painter = new Painter(m_display, m_window);
}

Widget::~Widget() {
  XDestroyWindow(m_display, m_window);
}

const bool Widget::isFocused() const {
  return Application::instance()->isFocused(id());
}

bool Widget::handleEvent(XEvent &e) {
  switch (e.type) {
  case KeyPress:
  case KeyRelease: {
    KeyEvent key = getKeyEvent(e);
    if (e.type == KeyPress)
      return keyPressEvent(key);
    if (e.type == KeyRelease)
      return keyReleaseEvent(key);
    return false;
  }
  case ButtonPress:
    auto mouseEvent = getMouseEvent(e);
    if (mouseEvent.isScroll) {
      return mouseScrollEvent(mouseEvent);
    }
    return mousePressEvent(mouseEvent);
  }
  return false;
};

void Widget::setBg(ulong c) {
  m_bgColor = c;
}

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

void Widget::paintEvent() {
}

bool Widget::keyPressEvent(KeyEvent) {
  return false;
}

bool Widget::keyReleaseEvent(KeyEvent) {
  return false;
}

bool Widget::mousePressEvent(MouseEvent) {
  return false;
}

bool Widget::mouseScrollEvent(MouseEvent) {
  return false;
}
