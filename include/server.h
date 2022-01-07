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
  virtual ~Server() {}
  virtual void setup() {}
  virtual bool isEventPending() { return false; }
  virtual bool shouldClose(E) { return true; }
  virtual KeyEvent getKeyEvent(E) { return {}; }
  virtual Painter *createPainter(D) { return nullptr; }
  virtual MouseEvent getMouseEvent(E) { return {}; }
  virtual D newSubWindow(std::string) { return {}; }
  virtual int setColor(void *, std::string) { return {}; }
  virtual D newParentWindow(ParentWindowInfo) { return {}; }

  virtual void getNextEvent(E *) { }
  virtual void showWindow(D ) { }
  virtual void hideWindow(D ) { }
  virtual void destroyWindow(D ) { }
  virtual void changeFocus(E , D *) { }
  virtual void setWindowBg(D , std::string ) { }
  virtual void setWindowSize(D , uint , uint ) { }
  virtual void setWindowSizeAndPos(D , Rect ) { }

  virtual void closeFontArea() {  }
  virtual F getFontArea() { return {}; }
  virtual void setFontArea(std::string) { }

  virtual bool onMouse(E &) { return {}; }
  virtual bool onKeyUp(E &) { return {}; }
  virtual bool onKeyDown(E &) { return {}; }

  virtual MouseButton getButton(int) { return MouseButton::Unknown; }
  virtual WheelDirection getDirection(int) { return WheelDirection::Unknown; }
};
