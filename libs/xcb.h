#pragma once
#include "server.h"
#include "xcbft.h"
#include <xcb/xcb.h>

#ifndef SERVER_
#define SERVER_
typedef struct {
  long dpi;
  double ascent;
  xcbft_face_holder holder;
} XcbFontInfo;

using IColor = uint32_t;
using FontArea = XcbFontInfo;
using DrawableId = xcb_window_t;
using Event = xcb_generic_event_t *;

using WindowServer = Server<Event, DrawableId, FontArea>;
#endif

class XcbServer : public WindowServer {
public:
  void setup() override;
  bool isEventPending() override;
  bool shouldClose(Event) override;
  KeyEvent getKeyEvent(Event) override;
  Painter *createPainter(DrawableId) override;
  MouseEvent getMouseEvent(Event) override;
  DrawableId newSubWindow(std::string) override;
  int setColor(void *, std::string) override;
  DrawableId newParentWindow(ParentWindowInfo) override;

  bool getNextEvent(Event *) override;
  void showWindow(DrawableId) override;
  void hideWindow(DrawableId) override;
  void destroyWindow(DrawableId) override;
  DrawableId changeFocus(Event) override;
  void setWindowBg(DrawableId, std::string) override;
  void setWindowSize(DrawableId, uint, uint) override;
  void setWindowSizeAndPos(DrawableId, Rect) override;

  void closeFontArea() override;
  FontArea getFontArea() override;
  void setFontArea(std::string) override;

  bool onMouse(Event &) override;
  bool onKeyUp(Event &) override;
  bool onKeyDown(Event &) override;

  MouseButton getButton(int) override;
  WheelDirection getDirection(int) override;

private:
  FontArea m_fontArea;
  DrawableId m_mainWindow;
  xcb_screen_t *m_screen = nullptr;
  xcb_generic_event_t *m_event = nullptr;
  xcb_connection_t *m_connection = nullptr;
  xcb_intern_atom_reply_t *m_AtomWmDeleteWindow;
};
