#include "../include/widget.h"
#include "../include/application.h"
#include "../include/typedefs.h"

Widget::Widget() : m_server(Application::instance()->server()) {
}

const Window Widget::id() const {
  return m_drawable;
}

void Widget::newWindow() {
  m_drawable = m_server->newSubWindow(m_bgColor);
  m_painter = m_server->createPainter(m_drawable);
}

Widget::~Widget() {
  delete m_painter;
  // XDestroyWindow(m_display, m_drawable);
}

const bool Widget::isFocused() const {
  return Application::instance()->isFocused(id());
}

bool Widget::handleEvent(Event &e) {
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

void Widget::setBg(std::string c) {
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
  m_server->setWindowSizeAndPos(m_drawable, m_rect);
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
