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

Atom getWindowClosingAtom(Display *dpy, Window w) {
  Atom dm = XInternAtom(dpy, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(dpy, w, &dm, 1);
  return dm;
}

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

void loadXdbeExtension(Display *dpy) {
  int majorVersion, minorVersion;
  if (!XdbeQueryExtension(dpy, &majorVersion, &minorVersion))
    throw std::runtime_error("XDBE is not supported!!!");
}

ulong stringToKeysym(const char *key) {
  return XStringToKeysym(key);
}

int getScreens(Display *dpy, int use_anchors, int *left_x, int *right_x, int *top_y, int *bottom_y) {
  // Get currently focused window
  Window win = -1;
  int focus_status;
  XGetInputFocus(dpy, &win, &focus_status);

  if (win == PointerRoot || win == None)
    return -1;

  XWindowAttributes win_attr;
  XRRScreenResources *screen_res = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
  XGetWindowAttributes(dpy, win, &win_attr);

  int det_x = 0, det_y = 0, nmonitors = 0;

  XRRGetMonitors(dpy, win, 1, &nmonitors);

  for (int i = 0; i < nmonitors; i++) {
    XRRCrtcInfo *screen_info = XRRGetCrtcInfo(dpy, screen_res, screen_res->crtcs[i]);

    // If the window is on the ith screen in the x
    if ((det_x >= screen_info->x && det_x < (int)(screen_info->x + screen_info->width)) &&
        (det_y >= screen_info->y && det_y < (int)(screen_info->y + screen_info->height))) {
      *left_x = screen_info->x;
      *right_x = screen_info->x + screen_info->width;
      *top_y = screen_info->y;
      *bottom_y = screen_info->y + screen_info->height;
      free(screen_info);
      free(screen_res);
      return 0;
    }
    free(screen_info);
  }

  free(screen_res);
  // If the function has not returned yet, then it could not find a screen on which 'win' resides.
  return -1;
}

void setWindowProperties(Display *dpy, Window win, std::string name, std::string title) {
  if (title == "")
    title = name;
  char *cname = (char *)(name.c_str());
  XClassHint class_hints = {.res_name = cname, .res_class = cname};
  XWMHints wmhints = {.flags = StateHint, .initial_state = NormalState};

  XSetWMHints(dpy, win, &wmhints);
  XSetTransientForHint(dpy, win, win);
  XStoreName(dpy, win, title.c_str());
  XSetClassHint(dpy, win, &class_hints);
}

void setWindowBg(Display *dpy, Window w, std::string color) {
  auto c = Color::get(color);
  XSetWindowBackground(dpy, w, c.pixel);
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
