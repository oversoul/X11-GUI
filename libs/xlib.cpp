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
  setWindowProperties(m_dpy, m_mainWindow, winInfo.name, winInfo.title);
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

void Xlib::getMonitorSize(uint *w, uint *h) {
  int left_x = 0, right_x = 0, top_y = 0, bottom_y = 0;
  if (getScreens(m_dpy, 0, &left_x, &right_x, &top_y, &bottom_y) < 0) {
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
