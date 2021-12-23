#pragma once
#include <X11/Xlib.h>
#include <assert.h>

typedef struct {
  int x;
  int y;
  int w;
  int h;
} Rect;

class Widget {
public:
  virtual const Window id() const {
    assert(false && "Widget doesn't have an ID");
    return -1;
  }

  static Window createWindow(Display *dpy, Rect r, long emask, Window p = -1) {
    int screen = DefaultScreen(dpy);
    int depth = DefaultDepth(dpy, screen);
    Visual *visual = XDefaultVisual(dpy, screen);

    if (p == (long unsigned int)-1) {
      p = DefaultRootWindow(dpy);
    }

    XSetWindowAttributes attr = {
        .background_pixel = 0xFFFFFF,
        .event_mask = emask,
    };

    unsigned long mask = CWBackPixel | CWEventMask;
    return XCreateWindow(dpy, p, r.x, r.y, r.w, r.h, 0, depth, InputOutput, visual, mask, &attr);
  }

  void setFocus(bool focus) { m_focus = focus; }
  const bool hasFocus() const { return m_focus; }
  const bool isVisible() const { return m_visible; }

  virtual bool handleEvent(XEvent &e) {
    switch (e.type) {
    case KeyPress:
      return keyPressEvent(e.xkey);
    case KeyRelease:
      return keyReleaseEvent(e.xkey);
    }
    return false;
  };

  void setRect(Rect r) { m_rect = r; }
  void setWidth(int w) { m_rect.w = w; }
  void setHeight(int w) { m_rect.w = w; }
  void setPosition(int x, int y) {
    m_rect.x = x;
    m_rect.y = y;
  }

  const Rect getRect() const { return m_rect; }
  void setBackground(unsigned long color) {
    m_bgColor = color;
    XSetWindowBackground(m_display, m_window, color);
  }

  void updateSizeAndPos() { //
    XMoveResizeWindow(m_display, m_window, m_rect.x, m_rect.y, m_rect.w, m_rect.h);
  }

  virtual void paintEvent(XEvent &) {}
  virtual bool keyPressEvent(XKeyEvent &) { return false; }
  virtual bool keyReleaseEvent(XKeyEvent &) { return false; }

protected:
  bool m_focus = false;
  bool m_visible = true;
  Window m_window;
  Display *m_display = nullptr;
  unsigned long m_bgColor = 0xffffff;
  Rect m_rect = {.x = 0, .y = 0, .w = 1, .h = 1};

  /*

  virtual bool textInputEvent(&);
  virtual bool mouseDoubleClickEvent(&);
  virtual bool mouseMoveEvent(&);
  virtual bool mousePressEvent(&);
  virtual bool mouseReleaseEvent(&);
  virtual void paintEvent(&);
  virtual void resizeEvent(&);
  */
};
