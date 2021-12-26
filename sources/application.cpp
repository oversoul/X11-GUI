#include "../headers/application.h"
#include "../headers/painter.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdbe.h>
#include <stdexcept>
#include <unistd.h>
#define FPS 120

Application *Application::m_instance = nullptr;

Application::Application(std::string title, int width, int height) : m_width(width), m_height(height) {
  if (m_instance != nullptr)
    throw std::runtime_error("The program can have only one instance of Application");
  m_instance = this;

  m_display = XOpenDisplay(nullptr);

  XSetWindowAttributes attr = {
      .background_pixel = 0xFFFFFF,
      .event_mask = ExposureMask | KeyPressMask | ButtonPressMask,
  };

  m_window = Widget::createWindow(m_display, {0, 0, width, height}, attr);

  XWMHints wmhints = {.flags = StateHint, .initial_state = NormalState};
  XSetWMHints(m_display, m_window, &wmhints);

  // wmhints.flags = USPosition | PAspect | PMinSize | PMaxSize;
  XStoreName(m_display, m_window, title.c_str());

  int majorVersion, minorVersion;
  if (!XdbeQueryExtension(m_display, &majorVersion, &minorVersion)) {
    throw std::runtime_error("XDBE is not supported!!!");
  }

  m_wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(m_display, m_window, &m_wmDeleteMessage, 1);
}

Application *Application::instance() {
  return m_instance;
}

Application::~Application() {
  XUnmapWindow(m_display, m_window);
  XDestroyWindow(m_display, m_window);
  XCloseDisplay(m_display);
}

bool Application::eventPending() {
  return XPending(m_display);
}

void Application::clear() {
  XClearWindow(m_display, m_window);
}

void Application::setLayout(Layout &l) {
  m_layout = &l;
}

void Application::setLayout(Layout *l) {
  m_layout = l;
}

void Application::addWidget(Widget *w) {
  m_widgets.push_back(w);
}

bool Application::shouldExit() {
  return m_shouldClose;
}

void Application::exit() {
  m_shouldClose = true;
}

void Application::checkForExit() {
  if (m_event.type == ClientMessage) {
    if ((Atom)m_event.xclient.data.l[0] == m_wmDeleteMessage)
      exit();
    return;
  }
  if (m_event.type == KeyPress) {
    if (XK_Escape == XLookupKeysym(&m_event.xkey, 0))
      exit();
    return;
  }
}

void Application::processEvents() {
  checkForExit();

  m_layout->updatePosition();

  if (m_event.type == ButtonPress && getButton(m_event.xbutton.button) == MouseButton::Left) {
    m_focusedWindow = m_event.xbutton.window;
  }

  for (auto &w : m_layout->getWidgets()) {
    w->updateSizeAndPos();
    w->handleEvent(m_event);
    w->paintEvent(m_event);
  }
}

bool Application::isFocused(Window id) {
  return focusedWindow() == id;
}

void Application::exec() {
  m_focusedWindow = m_window;

  while (!m_shouldClose) {
    XNextEvent(m_display, &m_event);
    processEvents();
    usleep(1000 * 1000 / FPS);
  }
}

const int Application::width() const {
  return m_width;
}

const int Application::height() const {
  return m_height;
}
