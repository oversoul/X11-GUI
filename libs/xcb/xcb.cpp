#include "xcb.h"
#include "xcbpainter.h"
#include <iostream>
#include <stdexcept>
#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xproto.h>

WindowServer *newServer(ServerType type) {
  return new XcbServer;
}

void XcbServer::setup() {
  m_connection = xcb_connect(NULL, NULL);

  if (!m_connection) {
    fprintf(stderr, "ERROR: can't connect to an X server via xcb\n");
    exit(1);
  }
  // Get the first screen
  const xcb_setup_t *setup = xcb_get_setup(m_connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  m_screen = iter.data;

  xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(m_connection, 0, 16, "WM_DELETE_WINDOW");
  m_AtomWmDeleteWindow = xcb_intern_atom_reply(m_connection, cookie2, 0);
}

bool XcbServer::isEventPending() {
  return true;
}

bool XcbServer::shouldClose(Event event) {
  int type = (m_event->response_type & ~0x80);
  // not detected...
  if (type == XCB_CLIENT_MESSAGE) {
    return ((*(xcb_client_message_event_t *)event).data.data32[0] == (*m_AtomWmDeleteWindow).atom);
  }

  if (type != XCB_KEY_PRESS)
    return false;

  const xcb_key_press_event_t *kp = (const xcb_key_press_event_t *)m_event;
  int32_t k = (int32_t)kp->detail;
  return (k == 9);
}

KeyEvent XcbServer::getKeyEvent(Event) {
  return {};
}

Painter *XcbServer::createPainter(DrawableId d) {
  return new XcbPainter(m_connection, m_screen, d, m_fontArea);
}

MouseEvent XcbServer::getMouseEvent(Event) {
  xcb_button_press_event_t *kr = (xcb_button_press_event_t *)m_event;
  MouseButton button = getButton(kr->detail);
  return {
      .x = kr->event_x,
      .y = kr->event_y,
      .isScroll = button == MouseButton::Scroll,
      .button = button,
      .direction = getDirection(kr->detail),
  };
}

DrawableId XcbServer::newSubWindow(std::string) {
  DrawableId window = xcb_generate_id(m_connection);
  auto depth = XCB_COPY_FROM_PARENT;
  auto io = XCB_WINDOW_CLASS_INPUT_OUTPUT;
  auto visual = m_screen->root_visual;

  uint32_t mask = XCB_CW_EVENT_MASK;
  uint32_t values[] = {
      XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS |
          XCB_EVENT_MASK_KEY_RELEASE,
  };
  xcb_create_window(m_connection, depth, window, m_mainWindow, 0, 0, 1, 1, 0, io, visual, mask, values);
  return window;
}

int XcbServer::setColor(void *mem, std::string name) {
  std::string rs = name.substr(1, 2);
  std::string gs = name.substr(3, 2);
  std::string bs = name.substr(5, 2);

  uint16_t r = strtol(rs.c_str(), NULL, 16);
  uint16_t g = strtol(gs.c_str(), NULL, 16);
  uint16_t b = strtol(bs.c_str(), NULL, 16);

  auto cmap = m_screen->default_colormap;
  xcb_alloc_color_cookie_t cookie = xcb_alloc_color(m_connection, cmap, r << 8, g << 8, b << 8);
  xcb_alloc_color_reply_t *reply = xcb_alloc_color_reply(m_connection, cookie, 0);

  if (!reply)
    return -1;

  *((uint32_t *)mem) = reply->pixel;
  free(reply);
  return 0;
}

DrawableId XcbServer::newParentWindow(ParentWindowInfo info) {
  m_mainWindow = xcb_generate_id(m_connection);
  DrawableId parent = m_screen->root;
  auto depth = XCB_COPY_FROM_PARENT;
  auto io = XCB_WINDOW_CLASS_INPUT_OUTPUT;
  auto visual = m_screen->root_visual;
  uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t values[2] = {
      m_screen->white_pixel,
      XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS |
          XCB_EVENT_MASK_KEY_RELEASE,
  };
  xcb_create_window(m_connection, depth, m_mainWindow, parent, 0, 0, info.w, info.h, 0, io, visual, mask, values);

  xcb_get_geometry_cookie_t cookie = xcb_get_geometry(m_connection, parent);
  xcb_generic_error_t *e;
  xcb_get_geometry_reply_t *reply = xcb_get_geometry_reply(m_connection, cookie, &e);

  std::cout << "x: " << reply->x << " y: " << reply->y << " w: " << reply->width << " h: " << reply->height
            << std::endl;

  return m_mainWindow;
}

bool XcbServer::getNextEvent(Event *evt) {
  xcb_flush(m_connection);
  // return ((*evt = xcb_poll_for_event(m_connection)) != NULL);
  m_event = xcb_poll_for_event(m_connection);
  return (m_event != NULL);
}

void XcbServer::showWindow(DrawableId d) {
  xcb_map_window(m_connection, d);
}

void XcbServer::hideWindow(DrawableId d) {
  xcb_unmap_window(m_connection, d);
}

void XcbServer::destroyWindow(DrawableId d) {
  xcb_destroy_window(m_connection, d);
}

DrawableId XcbServer::changeFocus(Event _) {
  if (m_event == NULL)
    return 0;

  int type = (m_event->response_type & ~0x80);
  xcb_button_press_event_t *kr = (xcb_button_press_event_t *)m_event;

  if (type == XCB_BUTTON_PRESS && getMouseEvent(m_event).button == MouseButton::Left) {
    return kr->event;
  }

  return 0;
}

void XcbServer::setWindowBg(DrawableId, std::string) {
}

void XcbServer::setWindowSize(DrawableId d, uint w, uint h) {
  int x = 0, y = 0;
  xcb_size_hints_t hints;
  xcb_icccm_size_hints_set_size(&hints, 1, w, h);
  xcb_icccm_size_hints_set_min_size(&hints, w, h);
  xcb_icccm_size_hints_set_max_size(&hints, w, h);
  xcb_icccm_size_hints_set_position(&hints, 1, x, y);
  xcb_icccm_set_wm_size_hints(m_connection, d, XCB_ATOM_WM_NORMAL_HINTS, &hints);
}

void XcbServer::setWindowSizeAndPos(DrawableId d, Rect r) {
  const uint32_t values[4] = {r.x, r.y, r.w, r.h};
  auto props = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
  xcb_configure_window(m_connection, d, props, values);
}

void XcbServer::closeFontArea() {
  xcbft_face_holder_destroy(m_fontArea.holder);
}

FontArea XcbServer::getFontArea() {
  return m_fontArea;
}

void XcbServer::setFontArea(std::string name) {
  FcStrSet *fontsearch = xcbft_extract_fontsearch_list(name.c_str());
  xcbft_patterns_holder font_patterns = xcbft_query_fontsearch_all(fontsearch);
  FcStrSetDestroy(fontsearch);
  long dpi = xcbft_get_dpi(m_connection);
  auto faces = xcbft_load_faces(font_patterns, dpi);

  auto ascent = faces.faces[0]->size->metrics.ascender / 64.0;
  m_fontArea.dpi = dpi;
  m_fontArea.holder = faces;
  m_fontArea.ascent = ascent;
}

bool XcbServer::onMouse(Event &) {
  int type = (m_event->response_type & ~0x80);
  return (type == XCB_BUTTON_PRESS);
}

bool XcbServer::onKeyUp(Event &) {
  return false;
}

bool XcbServer::onKeyDown(Event &) {
  int type = (m_event->response_type & ~0x80);
  if (type != XCB_KEY_PRESS) {
    return false;
  }

  // const xcb_key_press_event_t *kp = (const xcb_key_press_event_t *)m_event;
  // std::cout << "Key pressed in window " << (k) << " esc: " <<  << std::endl;
  return false;
}

MouseButton XcbServer::getButton(int detail) {
  switch (detail) {
  case XCB_BUTTON_INDEX_1:
    return MouseButton::Left;
  case XCB_BUTTON_INDEX_2:
    return MouseButton::Middle;
  case XCB_BUTTON_INDEX_3:
    return MouseButton::Right;
  case XCB_BUTTON_INDEX_4:
  case XCB_BUTTON_INDEX_5:
    return MouseButton::Scroll;
  default:
    return MouseButton::Unknown;
  }
}

WheelDirection XcbServer::getDirection(int btn) {
  if (btn == 4)
    return WheelDirection::Up;
  if (btn == 5)
    return WheelDirection::Down;
  return WheelDirection::Unknown;
}
