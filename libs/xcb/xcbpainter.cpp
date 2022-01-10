#include "xcbpainter.h"
#include "color.h"
#include "typedefs.h"
#include <xcb/xcb.h>
#include <xcb/xproto.h>

static void testCookie(xcb_void_cookie_t cookie, xcb_connection_t *connection, const char *errMessage) {
  xcb_generic_error_t *error = xcb_request_check(connection, cookie);
  if (error) {
    fprintf(stderr, "ERROR: %s : %d\n", errMessage, error->error_code);
    xcb_disconnect(connection);
    exit(-1);
  }
}

XcbPainter::XcbPainter(xcb_connection_t *conn, xcb_screen_t *screen, DrawableId window, FontArea ft)
    : m_connection(conn), m_screen(screen), m_window(window), m_area(ft) {
  m_gc = xcb_generate_id(conn);

  uint32_t mask = XCB_GC_GRAPHICS_EXPOSURES;
  uint32_t values[1] = {0};

  xcb_void_cookie_t gcCookie = xcb_create_gc_checked(conn, m_gc, window, mask, values);
  testCookie(gcCookie, conn, "can't create gc");
}

XcbPainter::~XcbPainter() {
  xcb_void_cookie_t gcCookie = xcb_free_gc(m_connection, m_gc);
  testCookie(gcCookie, m_connection, "can't free gc");
}

void XcbPainter::drawString(const char *text, int x, int y, std::string color) {
  setForeground(color);
  utf_holder msg = char_to_uint32(text);
  IColor c = Color::get(color);
  xcb_render_color_t text_color;
  text_color.red = ((c >> 16)) * 0x101;         // Extract the RR byte
  text_color.green = ((c >> 8) & 0xFF) * 0x101; // Extract the GG byte
  text_color.blue = (c & 0xFFFF) * 0x101;
  text_color.alpha = 0xFFFF;

  xcbft_draw_text(m_connection, m_window, x, y + m_area.ascent / 2, msg, text_color, m_area.holder, m_area.dpi);
}

void XcbPainter::drawLine(int x1, int y1, int x2, int y2) {
  // XDrawLine(m_display, m_backBuffer, m_gc, x1, y1, x2, y2);
}

void XcbPainter::drawCircle(Rect r) {
  // XDrawArc(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h, 0, 360 * 64);
}

void XcbPainter::fillCircle(Rect r) {
  // XFillArc(m_display, m_backBuffer, m_gc, r.x, r.y, r.w, r.h, 0, 360 * 64);
}

void XcbPainter::drawPoint(int x, int y) {
  // XDrawPoint(m_display, m_backBuffer, m_gc, x, y);
}

void XcbPainter::clear(std::string color, Rect r) {
  setForeground(color);
  ushort w = static_cast<ushort>(r.w);
  ushort h = static_cast<ushort>(r.h);
  xcb_rectangle_t rect[] = {{0, 0, w, h}};
  xcb_poly_fill_rectangle(m_connection, m_window, m_gc, 1, rect);
}

void XcbPainter::drawRect(Rect r) {
  short x = static_cast<short>(r.x);
  short y = static_cast<short>(r.y);
  ushort w = static_cast<ushort>(r.w);
  ushort h = static_cast<ushort>(r.h);
  const xcb_rectangle_t rects[] = {{x, y, w, h}};
  xcb_void_cookie_t cookie = xcb_poly_rectangle(m_connection, m_window, m_gc, 1, rects);
  testCookie(cookie, m_connection, "Couldn't draw rectangle");
}

void XcbPainter::fillRect(Rect r) {
  short x = static_cast<short>(r.x);
  short y = static_cast<short>(r.y);
  ushort w = static_cast<ushort>(r.w);
  ushort h = static_cast<ushort>(r.h);
  const xcb_rectangle_t rects[] = {{x, y, w, h}};
  xcb_void_cookie_t cookie = xcb_poly_fill_rectangle(m_connection, m_window, m_gc, 1, rects);
  testCookie(cookie, m_connection, "Couldn't fill rectangle");
}

void XcbPainter::setForeground(std::string color) {
  auto c = Color::get(color);
  uint32_t values[] = {c};
  xcb_change_gc(m_connection, m_gc, XCB_GC_FOREGROUND, values);
}

uint XcbPainter::textWidth(const char *text) {

  // XGlyphInfo extents = {};
  // XftTextExtentsUtf8(m_display, m_area, (const FcChar8 *)text, strlen(text), &extents);
  // return extents.xOff;
  return 0;
}

void XcbPainter::swapBuffers() {
  // XdbeSwapInfo swap_info = {.swap_window = m_window, .swap_action = 0};
  // XdbeSwapBuffers(m_display, &swap_info, 1);
}
