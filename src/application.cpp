#include "application.h"
#include "impl.h"
#include "typedefs.h"
#include <unistd.h>
#define FPS 120

Application *Application::m_instance = nullptr;

Application::Application(std::string name, std::string title) : m_width(640), m_height(480) {
  if (m_instance != nullptr)
    throw std::runtime_error("The program can have only one instance of Application");
  m_instance = this;

  m_server = newServer();

  m_server->setup();
  m_color = new Color(m_server);

  ParentWindowInfo info{.name = name, .color = "#ffffff", .title = title, .w = m_width, .h = m_height};
  m_window = m_server->newParentWindow(info);

  setSize(m_width, m_height);

  m_font = new FontSystem(m_server, "DejaVu", 16);
}

const FontArea Application::getFontArea() const {
  return m_server->getFontArea();
}

WindowServer *Application::server() const {
  return m_server;
}

void Application::setType(std::string type) {
  // addWindowState(m_display, m_window, "ABOVE");
  // setWindowType(m_display, m_window, "UTILITY");
  // DOCKAPP needs proper initialization afterwards (not to seen in taskbar, but be seen in system-tray)
  // setWindowType(m_display, m_window, "DOCK");
  // setWindowType(m_display, m_window, "NOTIFICATION");
  // NOICONIFY is only relevant if there's NOTASKBAR, vice-versa, and is more important
  // setWindowType(m_display, m_window, "DIALOG");
  // (not to have minimize-icon) than NOMAXIMIZE, ALWAYSONTOP can be set later
  // setWindowType(m_display, m_window, "TOOLBAR");
  // setWindowType(m_display, m_window, "MENU");
  // setWindowType(m_display, m_window, "NORMAL"); // printf("%d\n",options);
}

Application *Application::instance() {
  return m_instance;
}

void Application::setBg(std::string color) {
  m_server->setWindowBg(m_window, color);
}

void Application::setFont(std::string name, uint size, std::string weight) {
  m_font->setFont(name, size, weight);
}

FontSystem *Application::font() {
  return m_font;
}

void Application::setSize(uint w, uint h) {
  m_width = w;
  m_height = h;
  m_server->setWindowSize(m_window, w, h);
}

uint Application::width() const {
  return m_width;
}

uint Application::height() const {
  return m_height;
}

DrawableId Application::window() const {
  return m_window;
}

DrawableId Application::id() const {
  return m_window;
}

Display *Application::display() const {
  assert("display is not used anymore");
  return nullptr;
}

DrawableId Application::focusedWindow() const {
  return m_focusedWindow;
}

bool Application::isFocused(DrawableId id) {
  return focusedWindow() == id;
}

Application::~Application() {
  delete m_font;
  delete m_color;
  // delete m_server;
}

void Application::setLayout(Layout &l) {
  m_layout = &l;
}

void Application::setLayout(Layout *l) {
  m_layout = l;
}

bool Application::shouldExit() {
  return m_shouldClose;
}

void Application::triggerExit() {
  m_shouldClose = true;
}

void Application::checkForExit() {
  if (m_server->shouldClose(m_event))
    return triggerExit();
}

void Application::processEvents() {
  m_layout->updatePosition();

  /*
  if (m_event.type == ConfigureNotify) {
    setSize(m_width, m_height);
    if ((uint)m_event.xconfigure.width != m_width || (uint)m_event.xconfigure.height != m_height) {
      // if ((event.xconfigure.x!=ewin->x || event.xconfigure.y!=ewin->y)) break;
      // ewin->resizecnt++;    // ewin->x=event.xconfigure.x; ewin->y=event.xconfigure.y;
      m_width = m_event.xconfigure.width;
      m_height = m_event.xconfigure.height;
    }
    return;
  }
  */

  // std::cout << "pointer of focused: " << m_focusedWindow << std::endl;
  auto toFocus = m_server->changeFocus(m_event);
  if (toFocus != 0)
    m_focusedWindow = toFocus;

  for (auto &w : m_layout->getWidgets()) {
    w->updateSizeAndPos();
    if (!w->handleEvent(m_event))
      checkForExit();
    w->paintEvent();
  }
}

void Application::exec() {
  m_server->showWindow(m_window);
  for (auto &w : m_layout->getWidgets())
    m_server->showWindow(w->id());

  auto *widget = m_layout->getFirstWidget();

  // std::cout << "parent: " << m_focusedWindow << " window: " << m_window << std::endl;
  m_focusedWindow = widget == nullptr ? m_window : widget->id();
  while (!m_shouldClose) {
    while (m_server->getNextEvent(&m_event)) {
      processEvents();
    }

    usleep(1000 * 1000 / FPS);
  }
}
