#include "../headers/widget.h"
#include "../headers/application.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

const Window Widget::id() const {
  assert(false && "Widget doesn't have an ID");
  return -1;
}

MouseButton getButton(int btn) {
  if (btn == 1)
    return MouseButton::Left;
  if (btn == 2)
    return MouseButton::Middle;
  if (btn == 3)
    return MouseButton::Right;
  return MouseButton::Unknown;
}

Window Widget::createWindow(Display *dpy, Rect r, XSetWindowAttributes attr, Window p) {
  int screen = DefaultScreen(dpy);
  int depth = DefaultDepth(dpy, screen);
  Visual *visual = XDefaultVisual(dpy, screen);

  if (p == (long unsigned int)-1) {
    p = DefaultRootWindow(dpy);
  }

  unsigned long mask = CWBackPixel | CWEventMask;
  return XCreateWindow(dpy, p, r.x, r.y, r.w, r.h, 0, depth, InputOutput, visual, mask, &attr);
}

const bool Widget::isVisible() const { //
  return m_visible;
}

bool Widget::handleEvent(XEvent &e) {
  switch (e.type) {
  case KeyPress:
  case KeyRelease:
    KeySym key;
    char text[255];
    if (XLookupString(&e.xkey, text, 255, &key, 0) == 1) {
      std::string str(text);
      if (e.type == KeyPress)
        return keyPressEvent(key, str);
      else if (e.type == KeyRelease)
        return keyReleaseEvent(key, str);
    }
    return false;
  case ButtonPress: {
    return mousePressEvent(e.xbutton, getButton(e.xbutton.button));
  }
  default:
    paintEvent(e);
    return true;
  }
  return false;
};

void Widget::setRect(Rect r) { m_rect = r; }
void Widget::setWidth(int w) { m_rect.w = w; }
void Widget::setHeight(int h) { m_rect.h = h; }
void Widget::setPosition(int x, int y) {
  m_rect.x = x;
  m_rect.y = y;
}

const Rect Widget::getRect() const { //
  return m_rect;
}

void Widget::updateSizeAndPos() { //
  XMoveResizeWindow(m_display, m_window, m_rect.x, m_rect.y, m_rect.w, m_rect.h);
}

void Widget::paintEvent(XEvent &) {}
bool Widget::keyPressEvent(KeySym, std::string) { return false; }
bool Widget::keyReleaseEvent(KeySym, std::string) { return false; }
bool Widget::mousePressEvent(XButtonEvent &, MouseButton) { return false; }
