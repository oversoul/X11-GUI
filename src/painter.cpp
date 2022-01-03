#include "../include/painter.h"
#include "../include/application.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <cstring>
#include <iostream>

Painter::Painter(Display *display, Window window) : m_window(window), m_display(display) {
  m_gc = XCreateGC(m_display, m_window, 0, 0);
  int s = XDefaultScreen(display);

  m_backBuffer = XdbeAllocateBackBufferName(m_display, m_window, 0);
  m_draw = XftDrawCreate(display, m_backBuffer, DefaultVisual(display, s), DefaultColormap(display, s));

  initializeColor("#000000");
  initializeColor("#FFFFFF");
}

Painter::~Painter() {
  XftDrawDestroy(m_draw);
  XFreeGC(m_display, m_gc);
}

void Painter::initializeColor(std::string color) {
  if (m_colors.count(color))
    return;

  int s = XDefaultScreen(m_display);
  int cm = XDefaultColormap(m_display, s);
  Visual *dv = DefaultVisual(m_display, s);

  memset(&m_colors[color], 0, sizeof(XftColor));

  if (!XftColorAllocName(m_display, dv, cm, color.c_str(), &m_colors[color])) {
    XftDrawDestroy(m_draw);
    throw std::runtime_error("Couldn't create xft drawing area.");
  }
}

void Painter::drawString(const char *text, int x, int y, std::string color) {
  auto font = Application::instance()->font()->getFontArea();
  XftColor c = (m_colors.count(color)) ? m_colors[color] : m_colors["#000000"];
  XftDrawStringUtf8(m_draw, &c, font, x, y + font->ascent / 2, (FcChar8 *)text, strlen(text));
}

void Painter::drawLine(int x1, int y1, int x2, int y2) {
  XDrawLine(m_display, m_backBuffer, m_gc, x1, y1, x2, y2);
}

void Painter::drawCircle(Rect r) {
  XDrawArc(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h, 0, 360 * 64);
}

void Painter::fillCircle(Rect r) {
  XFillArc(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h, 0, 360 * 64);
}

void Painter::drawPoint(int x, int y) {
  XDrawPoint(m_display, m_backBuffer, m_gc, x, y);
}

void Painter::clear(unsigned long color) {
  XSetForeground(m_display, m_gc, color);
  XFillRectangle(m_display, m_backBuffer, m_gc, 0, 0, Application::instance()->width(),
                 Application::instance()->height());
}

void Painter::drawRect(Rect r) {
  XDrawRectangle(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h);
}

void Painter::fillRect(Rect r) {
  XFillRectangle(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h);
}

void Painter::setForeground(unsigned long color) {
  XSetForeground(m_display, m_gc, color);
}

void Painter::setBackground(unsigned long color) {
  XSetWindowBackground(m_display, m_window, color);
}

uint Painter::textWidth(const char *text) {
  XGlyphInfo extents = {};
  auto font = Application::instance()->font()->getFontArea();
  XftTextExtentsUtf8(m_display, font, (FcChar8 *)text, strlen(text), &extents);
  return extents.width;
}

void Painter::swapBuffers() {
  XdbeSwapInfo swap_info = {.swap_window = m_window, .swap_action = 0};
  XdbeSwapBuffers(m_display, &swap_info, 1);
}
