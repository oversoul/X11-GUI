#include "../headers/application.h"
#include "../headers/painter.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Application::Application(std::string title, int width, int height) {
  m_display = XOpenDisplay((char *)0);
  m_screen = DefaultScreen(m_display);
  m_root = DefaultRootWindow(m_display);

  unsigned long black = XBlackPixel(m_display, m_screen);
  unsigned long white = XWhitePixel(m_display, m_screen);
  m_window = XCreateSimpleWindow(m_display, m_root, 0, 0, width, height, 0, black, white);
  XSetStandardProperties(m_display, m_window, title.c_str(), "Hi", None, nullptr, 0, nullptr);

  setInputSelection();
  m_gc = XCreateGC(m_display, m_window, 0, 0);
  clear();
  setBackground(white);
  setForeground(black);

  m_painter = new Painter(this);
  m_wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(m_display, m_window, &m_wmDeleteMessage, 1);
}

Application::~Application() {
  XFreeGC(m_display, m_gc);
  XDestroyWindow(m_display, m_window);
  XCloseDisplay(m_display);
}

void Application::clear() {
  XClearWindow(m_display, m_window);
  XMapRaised(m_display, m_window);
}

void Application::redraw() { //
  XClearWindow(m_display, m_window);
}

void Application::setBackground(unsigned long color) { //
  XSetBackground(m_display, m_gc, color);
}

void Application::setForeground(unsigned long color) { //
  XSetBackground(m_display, m_gc, color);
}

void Application::setInputSelection(long types) { //
  XSelectInput(m_display, m_window, types);
}

XEvent Application::fetchEvent() {
  XNextEvent(m_display, &m_event);

  if (m_event.type == ClientMessage) {
    if ((Atom)m_event.xclient.data.l[0] == m_wmDeleteMessage) {
      m_shouldClose = true;
    }
  }

  return m_event;
}

bool Application::shouldExit() { //
  return m_shouldClose;
}

void Application::exit() { //
  m_shouldClose = true;
}

