#pragma once
#include "../include/painter.h"
#include "../include/rect.h"
#include "../include/server.h"
#include "../include/typedefs.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef SERVER_
#define SERVER_
using Event = sf::Event;
using IColor = sf::Color;
using FontArea = sf::Font;
using DrawableId = uint32_t;

using WindowServer = Server<Event, DrawableId, FontArea>;
#endif

class SfmlServer : public WindowServer {
public:
  ~SfmlServer();

  bool isEventPending() override;
  bool shouldClose(Event evt) override;
  KeyEvent getKeyEvent(Event e) override;
  MouseEvent getMouseEvent(Event e) override;
  Painter *createPainter(DrawableId d) override;
  int setColor(void *mem, std::string name) override;
  DrawableId newSubWindow(std::string color) override;
  DrawableId newParentWindow(ParentWindowInfo winInfo) override;

  void setup() override;
  bool getNextEvent(Event *evt) override;
  void showWindow(DrawableId w) override;
  void hideWindow(DrawableId w) override;
  void destroyWindow(DrawableId d) override;
  DrawableId changeFocus(Event e) override;
  void setWindowSizeAndPos(DrawableId d, Rect r) override;
  void setWindowSize(DrawableId d, uint w, uint h) override;
  void setWindowBg(DrawableId d, std::string color) override;

  // font...
  void closeFontArea() override;
  FontArea getFontArea() override;
  void setFontArea(std::string name) override;

  bool onMouse(Event &e) override;
  bool onKeyUp(Event &e) override;
  bool onKeyDown(Event &e) override;

  MouseButton getButton(int btn) override;
  WheelDirection getDirection(int btn) override;

private:
  sf::RenderWindow *m_mainWindow = nullptr;
};