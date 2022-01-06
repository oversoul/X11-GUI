#include "xlib.h"
#include "color.h"
#include "typedefs.h"
#include "xlibpainter.h"
#include <X11/Xlib.h>
#include <cstdlib>
#include <stdio.h>

Xlib::~Xlib() {
  hideWindow(m_mainWindow);
  XDestroyWindow(m_dpy, m_mainWindow);
  XCloseDisplay(m_dpy);
}

void Xlib::setup() {
  m_dpy = XOpenDisplay(getenv("DISPLAY"));
  if (!m_dpy) {
    fprintf(stderr, "Couldn't open Display\n");
    exit(1);
  }

  int majorVersion, minorVersion;
  if (!XdbeQueryExtension(m_dpy, &majorVersion, &minorVersion))
    throw std::runtime_error("XDBE is not supported!!!");

  m_defaultScreen = DefaultScreen(m_dpy);
}

DrawableId Xlib::newParentWindow(ParentWindowInfo winInfo) {
  m_mainWindow = createWindow(m_dpy, winInfo.color);

  if (winInfo.title == "")
    winInfo.title = winInfo.name;

  char *cname = (char *)(winInfo.name.c_str());
  XClassHint class_hints = {.res_name = cname, .res_class = cname};
  XWMHints wmhints = {.flags = StateHint, .initial_state = NormalState};

  XSetWMHints(m_dpy, m_mainWindow, &wmhints);
  XSetTransientForHint(m_dpy, m_mainWindow, m_mainWindow);
  XStoreName(m_dpy, m_mainWindow, winInfo.title.c_str());
  XSetClassHint(m_dpy, m_mainWindow, &class_hints);

  m_wmDeleteMessage = XInternAtom(m_dpy, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(m_dpy, m_mainWindow, &m_wmDeleteMessage, 1);
  getMonitorSize(&m_monitorWidth, &m_monitorHeight);
  return m_mainWindow;
}

DrawableId Xlib::newSubWindow(std::string color) {
  return createWindow(m_dpy, color, m_mainWindow);
}

Painter *Xlib::createPainter(DrawableId d) {
  return new XlibPainter(m_dpy, d, m_fontArea);
}

void Xlib::setWindowBg(DrawableId d, std::string color) {
  auto c = Color::get(color);
  XSetWindowBackground(m_dpy, d, c.pixel);
}

void Xlib::setWindowSizeAndPos(DrawableId d, Rect r) {
  XMoveResizeWindow(m_dpy, d, r.x, r.y, r.w, r.h);
}

void Xlib::hideWindow(DrawableId w) {
  XUnmapWindow(m_dpy, w);
}

void Xlib::showWindow(DrawableId w) {
  XMapWindow(m_dpy, w);
}

void Xlib::setWindowSize(DrawableId d, uint w, uint h) {
  uint x = m_monitorWidth / 2 - w / 2;
  uint y = m_monitorHeight / 2 - h / 2;

  XSizeHints sizehints;
  sizehints.flags = PMinSize | PMaxSize;
  sizehints.max_width = sizehints.min_width = w;
  sizehints.max_height = sizehints.min_height = h;
  XSetNormalHints(m_dpy, d, &sizehints);
  XMoveResizeWindow(m_dpy, d, x, y, w, h);
}

int Xlib::getScreens(int *left_x, int *right_x, int *top_y, int *bottom_y) {
  // Get currently focused window
  Window win = -1;
  int focus_status;
  XGetInputFocus(m_dpy, &win, &focus_status);

  if (win == PointerRoot || win == None)
    return -1;

  XWindowAttributes win_attr;
  XRRScreenResources *screen_res = XRRGetScreenResources(m_dpy, DefaultRootWindow(m_dpy));
  XGetWindowAttributes(m_dpy, win, &win_attr);

  int det_x = 0, det_y = 0, nmonitors = 0;

  XRRGetMonitors(m_dpy, win, 1, &nmonitors);

  for (int i = 0; i < nmonitors; i++) {
    XRRCrtcInfo *screen_info = XRRGetCrtcInfo(m_dpy, screen_res, screen_res->crtcs[i]);

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

void Xlib::getMonitorSize(uint *w, uint *h) {
  int left_x = 0, right_x = 0, top_y = 0, bottom_y = 0;
  if (getScreens(&left_x, &right_x, &top_y, &bottom_y) < 0) {
    *w = XDisplayWidth(m_dpy, m_defaultScreen);
    *h = XDisplayHeight(m_dpy, m_defaultScreen);
  } else {
    *w = right_x;
    *h = bottom_y;
  }
}

int Xlib::setColor(void *mem, std::string name) {
  int cm = XDefaultColormap(m_dpy, m_defaultScreen);
  Visual *dv = DefaultVisual(m_dpy, m_defaultScreen);
  memset((XftColor *)mem, 0, sizeof(XftColor));

  if (!XftColorAllocName(m_dpy, dv, cm, name.c_str(), (XftColor *)mem)) {
    return -1;
  }
  return 0;
}

bool Xlib::shouldClose(Event evt) {
  if (evt.type == ClientMessage) {
    return ((Atom)evt.xclient.data.l[0] == m_wmDeleteMessage);
  }

  return (evt.type == KeyPress && XK_Escape == XLookupKeysym(&evt.xkey, 0));
}

FontArea Xlib::getFontArea() {
  return m_fontArea;
}

void Xlib::setFontArea(std::string name) {
  closeFontArea();
  m_fontArea = XftFontOpenName(m_dpy, m_defaultScreen, name.c_str());

  if (!m_fontArea) {
    fprintf(stderr, "Couldn't open font.\n");
    exit(1);
  }
}

void Xlib::closeFontArea() {
  if (m_fontArea != nullptr)
    XftFontClose(m_dpy, m_fontArea);
}

void Xlib::getNextEvent(Event *evt) {
  XNextEvent(m_dpy, evt);
}

bool Xlib::isEventPending() {
  return XPending(m_dpy);
}

void Xlib::changeFocus(Event e, DrawableId *d) {
  if (e.type == ButtonPress && getMouseEvent(e).button == MouseButton::Left) {
    *d = e.xbutton.window;
  }
}
