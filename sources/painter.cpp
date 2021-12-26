#include "../headers/painter.h"
#include "../headers/application.h"
#include <X11/Xlib.h>
#include <cstring>

Painter::Painter(Display *display, Window window) : m_window(window), m_display(display) {
  m_gc = XCreateGC(m_display, m_window, 0, 0);
  m_font = XLoadQueryFont(m_display, "-*-clean-*-*-normal-*-15-150-*-*-*-*-*-*");
  m_backBuffer = XdbeAllocateBackBufferName(m_display, m_window, 0);

  if (!m_font) {
    printf("Failed to load font!\n");
    m_font = XLoadQueryFont(m_display, "fixed");
  }

  XSetFont(m_display, m_gc, m_font->fid);
}

Painter::~Painter() {
  XFreeFont(m_display, m_font);
  XFreeGC(m_display, m_gc);
}

void Painter::drawString(const char *text, int x, int y) {
  XDrawString(m_display, m_backBuffer, m_gc, x, y + (m_font->ascent / 2), text, strlen(text));
}

void Painter::drawLine(int x1, int y1, int x2, int y2) {
  XDrawLine(m_display, m_backBuffer, m_gc, x1, y1, x2, y2);
}

void Painter::drawPoint(int x, int y) {
  XDrawPoint(m_display, m_backBuffer, m_gc, x, y);
}

void Painter::clear() {
  XSetForeground(m_display, m_gc, 0xffffff);
  XFillRectangle(m_display, m_backBuffer, m_gc, 0, 0, Application::instance()->width(),
                 Application::instance()->height());
}

void Painter::drawRect(int x, int y, int width, int height) {
  XDrawRectangle(m_display, m_backBuffer, m_gc, x, y, width, height);
}

void Painter::fillRect(int x, int y, int width, int height) {
  XFillRectangle(m_display, m_backBuffer, m_gc, x, y, width, height);
}

void Painter::setForeground(unsigned long color) {
  XSetForeground(m_display, m_gc, color);
}

void Painter::setBackground(unsigned long color) {
  XSetWindowBackground(m_display, m_window, color);
}

void Painter::swapBuffers() {
  XdbeSwapInfo swap_info = {.swap_window = m_window, .swap_action = 0};
  XdbeSwapBuffers(m_display, &swap_info, 1);
}
