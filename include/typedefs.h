#pragma once
#include "rect.h"
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

typedef struct {
  KeySym key;
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

Display *openDisplay();
KeyEvent getKeyEvent(XEvent e);
MouseEvent getMouseEvent(XEvent e);
XftDraw *getXftDrawArea(Display *dpy, Drawable d);
void loadXdbeExtension(Display *dpy);
int getScreens(Display *dpy, int use_anchors, int *left_x, int *right_x, int *top_y, int *bottom_y);
Window createWindow(Display *dpy, std::string color, Window p = -1);
void setWindowProperties(Display *dpy, Window win, std::string name, std::string title);
void setWindowBg(Display *dpy, Window w, std::string color);
void setWindowSize(Display *dpy, Window win, uint x, uint y, uint w, uint h);
Atom getWindowClosingAtom(Display *dpy, Window w);

int setXftColor(Display *dpy, void *mem, std::string color);

void setWindowType(Display *dpy, Window w, std::string type);
Atom addWindowState(Display *dpy, Window w, std::string type);
Atom changeWMprop(Display *dpy, Window w, std::string property, const char *data, Bool overwrite);

ulong stringToKeysym(const char *key);
