#include "../include/painter.h"
#include "../include/application.h"
#include "../include/typedefs.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <cstring>
#include <iostream>

Painter::Painter(Display *display, Window window) : m_window(window), m_display(display) {
  m_gc = XCreateGC(m_display, m_window, 0, 0);
  int s = XDefaultScreen(display);
  m_font = XftFontOpenName(display, s, "SF Mono-12");
  if (!m_font) {
    fprintf(stderr, "Couldn't open font.\n");
    exit(1);
  }

  m_backBuffer = XdbeAllocateBackBufferName(m_display, m_window, 0);
  m_draw = createXftDraw(m_display, m_backBuffer);

  memset(&m_color, 0, sizeof(XftColor));

  if (!XftColorAllocName(display, DefaultVisual(display, s), DefaultColormap(display, s), "#000000", &m_color)) {
    XftDrawDestroy(m_draw);
    throw std::runtime_error("Couldn't create xft drawing area.");
  }
}

Painter::~Painter() {
  // XftDrawDestroy(m_draw);
  XFreeGC(m_display, m_gc);
}

void Painter::drawString(const char *text, int x, int y) {
  XftDrawStringUtf8(m_draw, &m_color, m_font, x, y + m_font->ascent / 2, (FcChar8 *)text, strlen(text));
}

void Painter::drawLine(int x1, int y1, int x2, int y2) {
  XDrawLine(m_display, m_backBuffer, m_gc, x1, y1, x2, y2);
}

void Painter::drawCircle(int x, int y, uint width, uint height) {
  XDrawArc(m_display, m_backBuffer, m_gc, x, y, width, height, 0, 360 * 64);
}

void Painter::fillCircle(int x, int y, uint width, uint height) {
  XFillArc(m_display, m_backBuffer, m_gc, x, y, width, height, 0, 360 * 64);
}

void Painter::drawPoint(int x, int y) {
  XDrawPoint(m_display, m_backBuffer, m_gc, x, y);
}

void Painter::clear(unsigned long color) {
  XSetForeground(m_display, m_gc, color);
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

unsigned int Painter::textWidth(const char *text) {
  XGlyphInfo extents = {};
  XftTextExtentsUtf8(m_display, m_font, (FcChar8 *)text, strlen(text), &extents);
  return extents.width;
}

void Painter::swapBuffers() {
  XdbeSwapInfo swap_info = {.swap_window = m_window, .swap_action = 0};
  XdbeSwapBuffers(m_display, &swap_info, 1);
}
