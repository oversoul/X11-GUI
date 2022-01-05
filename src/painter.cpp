#include "painter.h"
#include "application.h"
#include "color.h"

Painter::Painter(Display *display, Window window) : m_window(window), m_display(display) {
  m_gc = XCreateGC(m_display, m_window, 0, 0);

  m_backBuffer = XdbeAllocateBackBufferName(m_display, m_window, 0);
  m_draw = getXftDrawArea(m_display, m_backBuffer);
}

Painter::~Painter() {
  XdbeDeallocateBackBufferName(m_display, m_backBuffer);
  XftDrawDestroy(m_draw);
  XFreeGC(m_display, m_gc);
}

void Painter::drawString(const char *text, int x, int y, std::string color) {
  XftColor c = Color::get(color);
  auto font = Application::instance()->font()->getFontArea();
  XftDrawStringUtf8(m_draw, &c, font, x, y + font->ascent / 2, (const FcChar8 *)text, strlen(text));
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

void Painter::clear(std::string color, Rect r) {
  setForeground(color);
  XFillRectangle(m_display, m_backBuffer, m_gc, 0, 0, r.w, r.h);
}

void Painter::drawRect(Rect r) {
  XDrawRectangle(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h);
}

void Painter::fillRect(Rect r) {
  XFillRectangle(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h);
}

void Painter::setForeground(std::string color) {
  auto c = Color::get(color);
  XSetForeground(m_display, m_gc, c.pixel);
}

uint Painter::textWidth(const char *text) {
  XGlyphInfo extents = {};
  auto font = Application::instance()->font()->getFontArea();
  XftTextExtentsUtf8(m_display, font, (const FcChar8 *)text, strlen(text), &extents);
  return extents.width;
}

void Painter::swapBuffers() {
  XdbeSwapInfo swap_info = {.swap_window = m_window, .swap_action = 0};
  XdbeSwapBuffers(m_display, &swap_info, 1);
}
