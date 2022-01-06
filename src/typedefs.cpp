#include "typedefs.h"
#include "color.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>
#include <iostream>
#include <stdexcept>

// print key modifier...
void print_modifiers(uint mask) {
  const char *MODIFIERS[] = {"Shift", "Lock", "Ctrl", "Alt", "Mod2", "Mod3", "Mod4", "Mod5"};
  for (const char **modifier = MODIFIERS; mask; mask >>= 1, ++modifier) {
    if (mask & 1) {
      printf("Modifier mask: %s\n", *modifier);
    }
  }
}

/*
static int error_handler(Display *d, XErrorEvent *e) {
  std::cerr << "Error minor code: " << e->minor_code << std::endl;
  std::cerr << "Error code: " << e->error_code << std::endl;
  return 0;
}
*/

KeyEvent getKeyEvent(XEvent e) {
  KeySym key;
  char text[255];
  XLookupString(&e.xkey, text, 255, &key, 0);
  return {.key = key, .text = std::string(text)};
}

static MouseButton getButton(int btn) {
  if (btn == 1)
    return MouseButton::Left;
  if (btn == 2)
    return MouseButton::Middle;
  if (btn == 3)
    return MouseButton::Right;
  if (btn == 4 || btn == 5)
    return MouseButton::Scroll;
  return MouseButton::Unknown;
}

static WheelDirection getDirection(int btn) {
  if (btn == 4)
    return WheelDirection::Up;
  if (btn == 5)
    return WheelDirection::Down;
  return WheelDirection::Unknown;
}

MouseEvent getMouseEvent(XEvent e) {
  auto button = getButton(e.xbutton.button);

  return {
      .x = e.xbutton.x,
      .y = e.xbutton.y,
      .isScroll = button == MouseButton::Scroll,
      .button = button,
      .direction = getDirection(e.xbutton.button),
  };
}

void setWindowSize(Display *dpy, Window win, uint x, uint y, uint w, uint h) {
  XSizeHints sizehints;
  sizehints.flags = PMinSize | PMaxSize;
  sizehints.max_width = sizehints.min_width = w;
  sizehints.max_height = sizehints.min_height = h;
  XSetNormalHints(dpy, win, &sizehints);
  XMoveResizeWindow(dpy, win, x, y, w, h);
}


Window createWindow(Display *dpy, std::string color, Window p) {
  int screen = XDefaultScreen(dpy);
  int depth = DefaultDepth(dpy, screen);
  Visual *visual = XDefaultVisual(dpy, screen);
  auto c = Color::get(color);

  XSetWindowAttributes attr{
      .background_pixel = c.pixel,
      .event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask | KeyPressMask | KeyReleaseMask,
  };

  if (p == (long unsigned int)-1) {
    p = DefaultRootWindow(dpy);
  }

  ulong mask = CWBackPixel | CWEventMask | CWOverrideRedirect;
  auto w = XCreateWindow(dpy, p, 0, 1, 1, 1, 0, depth, InputOutput, visual, mask, &attr);
  return w;
}

Atom changeWMprop(Display *dpy, Window w, std::string property, const char *data, Bool overwrite) {
  Atom wmatom = XInternAtom(dpy, data, False);
  Atom atom = XInternAtom(dpy, property.c_str(), False);
  XChangeProperty(dpy, w, atom, XA_ATOM, 32, overwrite ? PropModeReplace : PropModeAppend, (unsigned char *)&wmatom, 1);
  return wmatom;
}

Atom addWindowState(Display *dpy, Window w, std::string type) {
  std::string prop = "_NET_WM_STATE_" + type;
  return changeWMprop(dpy, w, "_NET_WM_STATE", prop.c_str(), False);
}

void setWindowType(Display *dpy, Window w, std::string type) {
  // called when creating window, preferably not called afterwards
  // 64 Window-Manager property text max. size
  std::string prop = "_NET_WM_WINDOW_TYPE_" + type;
  // printf("Setting window as type %s\n",type);
  changeWMprop(dpy, w, "_NET_WM_WINDOW_TYPE", prop.c_str(), True);
}
