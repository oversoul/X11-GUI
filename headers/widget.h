#pragma once
#include <X11/X.h>
#include <X11/Xlib.h>
#include <assert.h>
#include <iostream>

class Application;

typedef struct RectS {
  int x, y, w, h;

  void print_r() { std::cout << "x: " << x << " y: " << y << " w: " << w << " h: " << h << std::endl; }

  bool inRegion(int mx, int my, RectS r) {
    bool inX = x >= r.x && x <= r.h;
    bool inY = y >= r.y && y <= r.w;
    return inX && inY;
  }

} Rect;

enum class MouseButton { Unknown, Left, Right, Middle };

MouseButton getButton(int btn);

class Widget {
public:
  virtual const Window id() const;
  static Window createWindow(Display *dpy, Rect r, XSetWindowAttributes attr, Window p = -1);

  const bool isVisible() const;

  void setRect(Rect r);
  void setWidth(int w);
  void setHeight(int h);
  void setPosition(int x, int y);
  const Rect getRect() const;

  void updateSizeAndPos();
  bool handleEvent(XEvent &e);
  virtual void paintEvent(XEvent &);
  virtual bool keyPressEvent(KeySym, std::string);
  virtual bool keyReleaseEvent(KeySym, std::string);
  virtual bool mousePressEvent(XButtonEvent &, MouseButton);

protected:
  Window m_window;
  bool m_visible = true;
  bool m_needRepaint = true;
  Display *m_display = nullptr;
  unsigned long m_bgColor = 0xffffff;
  Rect m_rect = {.x = 0, .y = 0, .w = 1, .h = 1};

  /*

  virtual bool textInputEvent(&);
  virtual bool mouseDoubleClickEvent(&);
  virtual bool mouseMoveEvent(&);
  virtual bool mouseReleaseEvent(&);
  virtual void paintEvent(&);
  virtual void resizeEvent(&);
  */
};
