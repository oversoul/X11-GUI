#include "sfml.h"
#include "sfmlpainter.h"
#include <SFML/Graphics/CircleShape.hpp>

WindowServer *newServer(ServerType type) {
  return new SfmlServer();
}

SfmlServer::~SfmlServer() {
}

bool SfmlServer::isEventPending() {
  return true;
}

bool SfmlServer::shouldClose(Event evt) {
  if (evt.type == sf::Event::Closed)
    return true;
  return !m_mainWindow->isOpen();
}

KeyEvent SfmlServer::getKeyEvent(Event e) {
  return {};
}

MouseEvent SfmlServer::getMouseEvent(Event e) {
  return {};
}

Painter *SfmlServer::createPainter(DrawableId d) {
  return new SfmlPainter();
}

int SfmlServer::setColor(void *mem, std::string name) {
  return 0;
}

DrawableId SfmlServer::newSubWindow(std::string color) {
  return -1;
}

DrawableId SfmlServer::newParentWindow(ParentWindowInfo winInfo) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  std::cout << "w: " << winInfo.w << " h: " << winInfo.h << std::endl;
  m_mainWindow = new sf::RenderWindow(sf::VideoMode(winInfo.w, winInfo.h), winInfo.title, sf::Style::Default, settings);
  sf::View view(sf::FloatRect(0, 0, winInfo.w, winInfo.h));
  view.setSize(640, 480);
  m_mainWindow->setView(view);
  return 0;
}

void SfmlServer::setup() {
}

bool SfmlServer::getNextEvent(Event *evt) {
  m_mainWindow->clear();

  auto rect = sf::RectangleShape();
  rect.setSize(sf::Vector2f(120.f, 50.f));
  rect.setFillColor(sf::Color::Green);
  rect.setPosition(40, 30);

  sf::Font font;
  if (!font.loadFromFile("/home/ae/.fonts/Cascadia.ttf")) {
    std::cout << "Can't load font" << std::endl;
    exit(1);
  }

  sf::Text text;
  text.setFont(font);
  text.setPosition(40, 30);
  text.setCharacterSize(24);
  text.setString("Hello world");
  text.setFillColor(sf::Color::Red);

  m_mainWindow->draw(rect);
  m_mainWindow->draw(text);

  m_mainWindow->display();

  return m_mainWindow->pollEvent(*evt);
}

void SfmlServer::showWindow(DrawableId w) {
}

void SfmlServer::hideWindow(DrawableId w) {
}

void SfmlServer::destroyWindow(DrawableId d) {
}

DrawableId SfmlServer::changeFocus(Event e) {
  return {};
}

void SfmlServer::setWindowSizeAndPos(DrawableId d, Rect r) {
}

void SfmlServer::setWindowSize(DrawableId d, uint w, uint h) {
}

void SfmlServer::setWindowBg(DrawableId d, std::string color) {
}

void SfmlServer::closeFontArea() {
}

FontArea SfmlServer::getFontArea() {
  return {};
}

void SfmlServer::setFontArea(std::string name) {
}

bool SfmlServer::onMouse(Event &e) {
  return false;
}

bool SfmlServer::onKeyUp(Event &e) {
  return false;
}

bool SfmlServer::onKeyDown(Event &e) {
  return false;
}

MouseButton SfmlServer::getButton(int btn) {
  return MouseButton::Unknown;
}

WheelDirection SfmlServer::getDirection(int btn) {
  return WheelDirection::Unknown;
}
