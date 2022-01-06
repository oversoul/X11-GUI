#pragma once
#include "rect.h"
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

typedef struct {
  unsigned long key;
  std::string text;
} KeyEvent;

enum class MouseButton { Unknown, Left, Middle, Right, Scroll };
enum class WheelDirection { Up, Down, Unknown };
typedef struct {
  int x, y;
  bool isScroll;
  MouseButton button;
  WheelDirection direction;
} MouseEvent;

MouseButton getButton(int btn);
WheelDirection getDirection(int btn);
Window createWindow(Display *dpy, std::string color, Window p = -1);

/*
void setWindowSize(Display *dpy, Window win, uint x, uint y, uint w, uint h);
void setWindowType(Display *dpy, Window w, std::string type);
Atom addWindowState(Display *dpy, Window w, std::string type);
Atom changeWMprop(Display *dpy, Window w, std::string property, const char *data, Bool overwrite);
*/
