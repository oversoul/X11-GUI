#pragma once
#include "../include/painter.h"
#include "../include/rect.h"
#include "../include/typedefs.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <iostream>

using Event = XEvent;
using FontArea = XftFont *;
using DrawableId = Window;

typedef struct {
  std::string name;
  std::string color;
  std::string title;
  uint w, h;
} ParentWindowInfo;

class Xlib {
public:
  ~Xlib();

  bool isEventPending();
  bool shouldClose(Event evt);
  KeyEvent getKeyEvent(Event e);
  MouseEvent getMouseEvent(Event e);
  Painter *createPainter(DrawableId d);
  int setColor(void *mem, std::string name);
  DrawableId newSubWindow(std::string color);
  DrawableId newParentWindow(ParentWindowInfo winInfo);

  void setup();
  void getNextEvent(Event *evt);
  void showWindow(DrawableId w);
  void hideWindow(DrawableId w);
  void destroyWindow(DrawableId d);
  void changeFocus(Event e, DrawableId *d);
  void setParentWindowSize(uint w, uint h);
  void setWindowSizeAndPos(DrawableId d, Rect r);
  void setWindowSize(DrawableId d, uint w, uint h);
  void setWindowBg(DrawableId d, std::string color);

  // font...
  void closeFontArea();
  FontArea getFontArea();
  void setFontArea(std::string name);

  bool onMouse(Event &e);
  bool onKeyUp(Event &e);
  bool onKeyDown(Event &e);

  MouseButton getButton(int btn);
  WheelDirection getDirection(int btn);

private:
  void getMonitorSize(uint *w, uint *h);

  int getScreens(int *left_x, int *right_x, int *top_y, int *bottom_y);
  DrawableId createWindow(Display *dpy, std::string color, DrawableId p = -1);

  Display *m_dpy;
  int m_defaultDepth;
  int m_defaultScreen;
  int m_defaultColorMap;
  Atom m_wmDeleteMessage;
  Visual *m_defaultVisual;
  DrawableId m_mainWindow;
  FontArea m_fontArea = nullptr;
  uint m_monitorWidth, m_monitorHeight;
};
