#include "../include/widget.h"
#include "../include/application.h"
#include "../include/typedefs.h"

Widget::Widget() : m_server(Application::instance()->server()) {
}

const DrawableId Widget::id() const {
  return m_drawable;
}

void Widget::newWindow() {
  m_drawable = m_server->newSubWindow(m_bgColor);
  m_painter = m_server->createPainter(m_drawable);
}

Widget::~Widget() {
  delete m_painter;
  m_server->destroyWindow(m_drawable);
}

bool Widget::isFocused() const {
  return Application::instance()->isFocused(id());
}

bool Widget::handleEvent(Event &e) {
  if (isFocused()) {
    if (m_server->onKeyDown(e)) {
      auto ke = m_server->getKeyEvent(e);
      auto handled = keyPressEvent(ke);
      if (!handled) {
        if (ke.key == XK_Tab) {
          auto direction = ke.mod == KeyModifier::Shift ? -1 : 1;
          Application::instance()->focusNext(direction);
          return true;
        }
      }
      return false;
    }
    if (m_server->onKeyUp(e)) {
      auto key = m_server->getKeyEvent(e);
      return keyReleaseEvent(key);
    }
  }
  if (m_server->onHover(e)) {
    if (e.xany.window == Application::instance()->window()) {
      m_server->useCursor(CursorType::Normal);
    } else if (e.xany.window == id()) {
      mouseOverEvent();
    }
  }
  if (m_server->onMouse(e)) {
    auto mouseEvent = m_server->getMouseEvent(e);
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

void Widget::mouseOverEvent() {
  
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
