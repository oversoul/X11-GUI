#include "../include/application.h"
#include "../include/typedefs.h"
#include <unistd.h>
#define FPS 120

Application *Application::m_instance = nullptr;

Application::Application(std::string title, bool isModal) : m_width(640), m_height(480) {
  if (m_instance != nullptr)
    throw std::runtime_error("The program can have only one instance of Application");
  m_instance = this;

  m_display = XOpenDisplay(NULL);

  if (!m_display) {
    fprintf(stderr, "Couldn't open Display\n");
    exit(1);
  }

  getMonitorSize(m_display, &m_screenWidth, &m_screenHeight);

  XSetWindowAttributes attr = {
      .background_pixel = 0xFFFFFF,
      .event_mask = ExposureMask | KeyPressMask | ButtonPressMask,
      .override_redirect = isModal,
  };

  m_window = createWindow(m_display, {0, 0, 1, 1}, attr);
  setSize(m_width, m_height);
  if (isModal) {
    XSetInputFocus(m_display, m_window, RevertToPointerRoot, CurrentTime);
  } else {
    XSetTransientForHint(m_display, m_window, m_window);
  }

  XWMHints wmhints = {.flags = StateHint, .initial_state = NormalState};
  XSetWMHints(m_display, m_window, &wmhints);
  XStoreName(m_display, m_window, title.c_str());

  loadXdbeExtension(m_display);

  m_wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(m_display, m_window, &m_wmDeleteMessage, 1);
}

Application *Application::instance() {
  return m_instance;
}

void Application::setSize(unsigned int w, unsigned int h) {
  m_width = w;
  m_height = h;
  XMoveResizeWindow(m_display, m_window, m_screenWidth / 2 - w / 2, m_screenHeight / 2 - h / 2, w, h);
}

const unsigned int Application::width() const {
  return m_width;
}

const unsigned int Application::height() const {
  return m_height;
}

const Window Application::window() const {
  return m_window;
}

const Window Application::id() const {
  return m_window;
}

Display *Application::display() const {
  return m_display;
}

const Window Application::focusedWindow() const {
  return m_focusedWindow;
}

bool Application::isFocused(Window id) {
  return focusedWindow() == id;
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

void Application::triggerExit() {
  m_shouldClose = true;
}

void Application::checkForExit() {
  if (m_event.type == ClientMessage) {
    if ((Atom)m_event.xclient.data.l[0] == m_wmDeleteMessage)
      triggerExit();
    return;
  }
  if (m_event.type == KeyPress) {
    if (XK_Escape == XLookupKeysym(&m_event.xkey, 0))
      triggerExit();
    return;
  }
}

void Application::processEvents() {
  m_layout->updatePosition();

  if (m_event.type == ButtonPress && getButton(m_event.xbutton.button) == MouseButton::Left) {
    m_focusedWindow = m_event.xbutton.window;
  }

  for (auto &w : m_layout->getWidgets()) {
    w->updateSizeAndPos();
    if (!w->handleEvent(m_event)) {
      checkForExit();
    }
    w->paintEvent(m_event);
  }
}

void Application::exec() {
  auto *widget = m_layout->getFirstWidget();
  m_focusedWindow = widget == nullptr ? m_window : widget->id();

  while (!m_shouldClose) {
    XNextEvent(m_display, &m_event);
    processEvents();
    usleep(1000 * 1000 / FPS);
  }
}
