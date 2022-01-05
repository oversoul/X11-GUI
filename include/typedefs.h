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
int getScreen(Display *d);
KeyEvent getKeyEvent(XEvent e);
MouseEvent getMouseEvent(XEvent e);
XftDraw *getXftDrawArea(Display *dpy, Drawable d);
void loadXdbeExtension(Display *dpy);
int getScreens(Display *dpy, int use_anchors, int *left_x, int *right_x, int *top_y, int *bottom_y);
void getMonitorSize(Display *dpy, uint *width, uint *height);
Window createWindow(Display *dpy, ulong color, Window p = -1);
void setWindowNameAndTitle(Display *dpy, Window win, std::string name, std::string title);
Atom getWindowClosingAtom(Display *dpy, Window w);

int setXftColor(Display *dpy, void *mem, std::string color);

void setWindowType(Display *dpy, Window w, std::string type);
Atom addWindowState(Display *dpy, Window w, std::string type);
Atom changeWMprop(Display *dpy, Window w, std::string property, const char *data, Bool overwrite);

ulong stringToKeysym(const char *key);
