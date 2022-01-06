#pragma once
#include "../include/painter.h"
#include "../include/rect.h"
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
  void setup();
  void getMonitorSize(uint *w, uint *h);
  DrawableId newParentWindow(ParentWindowInfo winInfo);
  DrawableId newSubWindow(std::string color);
  void setParentWindowSize(uint w, uint h);
  void setWindowSizeAndPos(DrawableId d, Rect r);

  Painter *createPainter(DrawableId d);

  void showWindow(DrawableId w);
  void hideWindow(DrawableId w);

  bool shouldClose(Event evt);
  int setColor(void *mem, std::string name);
  void setWindowSize(DrawableId d, uint w, uint h);
  void setWindowBg(DrawableId d, std::string color);

  void getNextEvent(Event *evt);

  bool isEventPending();

  void changeFocus(Event e, DrawableId* d);

  // font...
  FontArea getFontArea();
  void setFontArea(std::string name);
  void closeFontArea();

private:
  int getScreens(int *left_x, int *right_x, int *top_y, int *bottom_y);
  int m_defaultScreen;
  uint m_monitorWidth, m_monitorHeight;
  Display *m_dpy;
  Atom m_wmDeleteMessage;
  DrawableId m_mainWindow;
  FontArea m_fontArea = nullptr;
};
