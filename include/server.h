#pragma once
#include "painter.h"
#include "typedefs.h"

typedef struct {
  std::string name;
  std::string color;
  std::string title;
  uint w, h;
} ParentWindowInfo;

enum class ServerType { Xlib, Xcb };

template <typename E, typename D, typename F> class Server {
public:
  virtual void setup() {}
  virtual bool isEventPending() { return false; }
  virtual bool shouldClose(E evt) { return true; }
  virtual KeyEvent getKeyEvent(E e) { return {}; }
  virtual Painter *createPainter(D d) { return nullptr; }
  virtual MouseEvent getMouseEvent(E e) { return {}; }
  virtual D newSubWindow(std::string color) { return {}; }
  virtual int setColor(void *mem, std::string name) { return {}; }
  virtual D newParentWindow(ParentWindowInfo winInfo) { return {}; }

  virtual void getNextEvent(E *evt) { }
  virtual void showWindow(D w) { }
  virtual void hideWindow(D w) { }
  virtual void destroyWindow(D d) { }
  virtual void changeFocus(E e, D *d) { }
  virtual void setWindowSizeAndPos(D d, Rect r) { }
  virtual void setWindowSize(D d, uint w, uint h) { }
  virtual void setWindowBg(D d, std::string color) { }

  virtual void closeFontArea() {  }
  virtual F getFontArea() { return {}; }
  virtual void setFontArea(std::string name) { }

  virtual bool onMouse(E &e) { return {}; }
  virtual bool onKeyUp(E &e) { return {}; }
  virtual bool onKeyDown(E &e) { return {}; }

  virtual MouseButton getButton(int btn) { return MouseButton::Unknown; }
  virtual WheelDirection getDirection(int btn) { return WheelDirection::Unknown; }
};
