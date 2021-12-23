#include "../headers/painter.h"
#include "../headers/application.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstring>

Painter::Painter(Display *display, Window window) : m_window(window), m_display(display) { //
  m_gc = XCreateGC(m_display, m_window, 0, 0);
  m_font = XLoadQueryFont(m_display, "-*-clean-*-*-normal-*-15-150-*-*-*-*-*-*");

  if (!m_font) {
    printf("Failed to load lucidasans-12 font!\n");
    m_font = XLoadQueryFont(m_display, "fixed");
  }

  XSetFont(m_display, m_gc, m_font->fid);

  clear();
}

Painter::~Painter() { //
  XFreeFont(m_display, m_font);
  XFreeGC(m_display, m_gc);
}

void Painter::drawString(const char *text, int x, int y) {
  XDrawString(m_display, m_window, m_gc, x, y + (m_font->ascent / 2), text, strlen(text));
}

void Painter::drawLine(int x1, int y1, int x2, int y2) { //
  XDrawLine(m_display, m_window, m_gc, x1, y1, x2, y2);
}

void Painter::drawPoint(int x, int y) { //
  XDrawPoint(m_display, m_window, m_gc, x, y);
}

void Painter::clear() { //
  XClearWindow(m_display, m_window);
}

void Painter::drawRect(int x, int y, int width, int height) {
  XDrawRectangle(m_display, m_window, m_gc, x, y, width, height);
}

void Painter::setForeground(unsigned long color) { //
  XSetForeground(m_display, m_gc, color);
}

void Painter::setBackground(unsigned long color) { //
  XSetBackground(m_display, m_gc, color);
}
