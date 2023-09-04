#pragma once
#include "../include/painter.h"
#include "../include/rect.h"
#include "../include/server.h"
#include "../include/typedefs.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <iostream>

#ifndef SERVER_
#define SERVER_
using Event = XEvent;
using IColor = XftColor;
using FontArea = XftFont *;
using DrawableId = Window;

using WindowServer = Server<Event, DrawableId, FontArea>;
#endif

class Xlib : public WindowServer {
public:
  ~Xlib();

  bool isEventPending() override;
  bool shouldClose(Event evt) override;
  KeyEvent getKeyEvent(Event e) override;
  MouseEvent getMouseEvent(Event e) override;
  Painter *createPainter(DrawableId d) override;
  int setColor(void *mem, std::string name) override;
  DrawableId newSubWindow(std::string color) override;
  DrawableId newParentWindow(ParentWindowInfo winInfo) override;

  std::string getClipboard() override;

  void setup() override;
  bool getNextEvent(Event *evt) override;
  void showWindow(DrawableId w) override;
  void hideWindow(DrawableId w) override;
  void destroyWindow(DrawableId d) override;
  DrawableId changeFocus(Event e) override;
  void setWindowSizeAndPos(DrawableId d, Rect r) override;
  void setWindowSize(DrawableId d, uint w, uint h) override;
  void setWindowBg(DrawableId d, std::string color) override;

  // font...
  void closeFontArea() override;
  FontArea getFontArea() override;
  void setFontArea(std::string name) override;

  bool onMouse(Event &e) override;
  bool onKeyUp(Event &e) override;
  bool onKeyDown(Event &e) override;

  MouseButton getButton(int btn) override;
  WheelDirection getDirection(int btn) override;

private:
  void getMonitorSize(uint *w, uint *h);

  int getScreens(int *left_x, int *right_x, int *top_y, int *bottom_y);
  DrawableId createWindow(Display *dpy, std::string color, DrawableId p = -1);

  Display *m_dpy;
  Atom m_clipboard;
  int m_defaultDepth;
  int m_defaultScreen;
  int m_defaultColorMap;
  Atom m_wmDeleteMessage;
  Visual *m_defaultVisual;
  DrawableId m_mainWindow;
  FontArea m_fontArea;
  uint m_monitorWidth, m_monitorHeight;
};
