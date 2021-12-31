#include "application.h"
#include "typedefs.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#define FPS 120

Application *Application::m_instance = nullptr;

Application::Application(std::string title, bool isModal) : m_width(640), m_height(480) {
  if (m_instance != nullptr)
    throw std::runtime_error("The program can have only one instance of Application");
  m_instance = this;

  m_display = XOpenDisplay(getenv("DISPLAY"));

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

  m_font = new FontSystem(m_display, "arial", 12);
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

void Application::setBg(unsigned long color) {
  XSetWindowBackground(m_display, m_window, color);
}

void Application::setFont(std::string name, uint size) {
  m_font->setFont(name, size);
}

FontSystem *Application::font() {
  return m_font;
}

void Application::setSize(uint w, uint h) {
  m_width = w;
  m_height = h;

  XSizeHints sizehints;
  sizehints.flags = PMinSize | PMaxSize;
  sizehints.min_width = w;
  sizehints.max_width = w;
  sizehints.min_height = h;
  sizehints.max_height = h;
  XSetNormalHints(m_display, m_window, &sizehints);
  XMoveResizeWindow(m_display, m_window, m_screenWidth / 2 - w / 2, m_screenHeight / 2 - h / 2, w, h);
}

const uint Application::width() const {
  return m_width;
}

const uint Application::height() const {
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

  if (m_event.type == ConfigureNotify) {
    /*
    setSize(m_width, m_height);
    if ((uint)m_event.xconfigure.width != m_width || (uint)m_event.xconfigure.height != m_height) {
      // if ((event.xconfigure.x!=ewin->x || event.xconfigure.y!=ewin->y)) break;
      // ewin->resizecnt++;    // ewin->x=event.xconfigure.x; ewin->y=event.xconfigure.y;
      m_width = m_event.xconfigure.width;
      m_height = m_event.xconfigure.height;
    }
    */
    return;
  }

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
    while (eventPending()) {
      XNextEvent(m_display, &m_event);
      processEvents();
    }
    usleep(1000 * 1000 / FPS);
  }
}
