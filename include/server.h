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
  virtual ~Server() {
  }
  virtual void setup() = 0;
  virtual bool isEventPending() = 0;
  virtual bool shouldClose(E) = 0;
  virtual KeyEvent getKeyEvent(E) = 0;
  virtual Painter *createPainter(D) = 0;
  virtual MouseEvent getMouseEvent(E) = 0;
  virtual D newSubWindow(std::string) = 0;
  virtual int setColor(void *, std::string) = 0;
  virtual D newParentWindow(ParentWindowInfo) = 0;

  virtual std::string getClipboard() = 0;

  virtual bool getNextEvent(E *) = 0;
  virtual void showWindow(D) = 0;
  virtual void hideWindow(D) = 0;
  virtual void destroyWindow(D) = 0;
  virtual D changeFocus(E) = 0;
  virtual void setWindowBg(D, std::string) = 0;
  virtual void setWindowSize(D, uint, uint) = 0;
  virtual void setWindowSizeAndPos(D, Rect) = 0;

  virtual void closeFontArea() = 0;
  virtual F getFontArea() = 0;
  virtual void setFontArea(std::string) = 0;

  virtual bool onMouse(E &) = 0;
  virtual bool onKeyUp(E &) = 0;
  virtual bool onKeyDown(E &) = 0;

  virtual MouseButton getButton(int) = 0;
  virtual WheelDirection getDirection(int) = 0;
};
