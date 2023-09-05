#include "xlibpainter.h"
#include "color.h"
#include "typedefs.h"
#include <X11/Xlib.h>

XlibPainter::XlibPainter(Display *display, Window window, FontArea area)
    : m_display(display), m_window(window), m_area(area) {
  m_gc = XCreateGC(m_display, m_window, 0, 0);

  m_backBuffer = XdbeAllocateBackBufferName(m_display, m_window, 0);

  int s = XDefaultScreen(display);
  m_draw = XftDrawCreate(display, m_backBuffer, DefaultVisual(display, s), DefaultColormap(display, s));
}

XlibPainter::~XlibPainter() {
  XdbeDeallocateBackBufferName(m_display, m_backBuffer);
  XftDrawDestroy(m_draw);
  XFreeGC(m_display, m_gc);
}

void XlibPainter::drawString(const char *text, int x, int y, std::string color) {
  IColor c = Color::get(color);
  XftDrawStringUtf8(m_draw, &c, m_area, x, y + m_area->ascent / 2, (const FcChar8 *)text, strlen(text));
}

void XlibPainter::drawLine(int x1, int y1, int x2, int y2, int w) {
  XSetLineAttributes(m_display, m_gc, w, LineSolid, CapRound, JoinRound);
  XDrawLine(m_display, m_backBuffer, m_gc, x1, y1, x2, y2);
  XSetLineAttributes(m_display, m_gc, 1, LineSolid, CapRound, JoinRound);
}

void XlibPainter::drawCircle(Rect r) {
  XDrawArc(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h, 0, 360 * 64);
}

void XlibPainter::fillCircle(Rect r) {
  XFillArc(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h, 0, 360 * 64);
}

void XlibPainter::roundedRect(Rect r, uint radius) {
  // Draw top left arc
  XDrawArc(m_display, m_backBuffer, m_gc, r.x, r.y, 2 * radius, 2 * radius, 90 * 64, 90 * 64);

  // Draw top right arc
  XDrawArc(m_display, m_backBuffer, m_gc, r.x + r.w - 2 * radius, r.y, 2 * radius, 2 * radius, 0 * 64, 90 * 64);

  // Draw bottom left arc
  XDrawArc(m_display, m_backBuffer, m_gc, r.x, r.y + r.h - 2 * radius, 2 * radius, 2 * radius, 180 * 64, 90 * 64);

  // Draw bottom right arc
  XDrawArc(m_display, m_backBuffer, m_gc, r.x + r.w - 2 * radius, r.y + r.h - 2 * radius, 2 * radius, 2 * radius, 270 * 64, 90 * 64);

  // Draw top side
  XDrawLine(m_display, m_backBuffer, m_gc, r.x + radius, r.y, r.w - radius, r.y);

  // Draw bottom side
  XDrawLine(m_display, m_backBuffer, m_gc, r.x + radius, r.y + r.h, r.w - radius, r.y + r.h);

  // Draw left side
  XDrawLine(m_display, m_backBuffer, m_gc, r.x, r.y + radius, r.x, r.h - radius);

  // Draw right side
  XDrawLine(m_display, m_backBuffer, m_gc, r.x + r.w, r.y + radius, r.w, r.h - radius);

  /*
  // Draw center
  XFillRectangle(m_display, m_backBuffer, m_gc, r.x + radius, r.y + radius, r.w - 2 * radius, r.h - 2 * radius);
*/
}

void XlibPainter::drawPoint(int x, int y) {
  XDrawPoint(m_display, m_backBuffer, m_gc, x, y);
}

void XlibPainter::clear(std::string color, Rect r) {
  setForeground(color);
  XFillRectangle(m_display, m_backBuffer, m_gc, 0, 0, r.w, r.h);
}

void XlibPainter::drawRect(Rect r) {
  XDrawRectangle(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h);
}

void XlibPainter::fillRect(Rect r) {
  XFillRectangle(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h);
}

void XlibPainter::setForeground(std::string color) {
  auto c = Color::get(color);
  XSetForeground(m_display, m_gc, c.pixel);
}

uint XlibPainter::textWidth(const char *text) {
  XGlyphInfo extents = {};
  XftTextExtentsUtf8(m_display, m_area, (const FcChar8 *)text, strlen(text), &extents);
  return extents.xOff;
}

void XlibPainter::swapBuffers() {
  XdbeSwapInfo swap_info = {.swap_window = m_window, .swap_action = 0};
  XdbeSwapBuffers(m_display, &swap_info, 1);
}
