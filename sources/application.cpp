#include "../headers/application.h"
#include "../headers/painter.h"
#include <X11/X.h>
#include <X11/Xlib.h>

Application *Application::m_instance = nullptr;

Application::Application(std::string title, int width, int height) {

  if (m_instance != nullptr)
    throw std::runtime_error("The program can have only one instance of Application");
  m_instance = this;

  m_display = XOpenDisplay(nullptr);

  Rect r = {0, 0, width, height};
  long mask = ExposureMask | KeyPressMask | ButtonPressMask;

  m_window = Widget::createWindow(m_display, r, mask);

  XWMHints wmhints = {
      .flags = StateHint,
      .initial_state = NormalState,
  };

  XTextProperty windowName;

  XSetWMHints(m_display, m_window, &wmhints);

  char *winTitle = (char *)title.c_str();
  XStringListToTextProperty(&winTitle, 1, &windowName);

  XSetWMName(m_display, m_window, &windowName);
  XMapWindow(m_display, m_window);
}

Application *Application::instance() { //
  return m_instance;
}

Application::~Application() {
  XDestroyWindow(m_display, m_window);
  XCloseDisplay(m_display);
}

bool Application::eventPending() { //
  return XPending(m_display);
}

void Application::clear() { //
  XClearWindow(m_display, m_window);
}

void Application::redraw() { //
  clear();
  XMapWindow(m_display, m_window);
  XMapRaised(m_display, m_window);

  m_layout->updatePosition();

  for (auto w_ : m_layout->getWidgets()) {
    w_->paintEvent(m_event);
  }
}

void Application::setLayout(Layout &l) { m_layout = &l; }
void Application::setLayout(Layout *l) { m_layout = l; }

void Application::setInputSelection(long types) { //
  m_input_types = types;
  XSelectInput(m_display, m_window, types);
}

long Application::getInputSelection() { //
  return m_input_types;
}

void Application::addWidget(Widget *w) { //
  m_widgets.push_back(w);
}

bool Application::shouldExit() { //
  return m_shouldClose;
}

void Application::exit() { //
  m_shouldClose = true;
}

void Application::exec() {
  Window window = -1;

  while (!m_shouldClose) {
    XNextEvent(m_display, &m_event);
    window = -1;

    if (m_window == m_event.xany.window) {
      window = m_window;
    } else {
      for (auto w_ : m_layout->getWidgets()) {
        if (w_->id() == m_event.xany.window) {
          window = w_->id();
          break;
        }
      }
    }

    (void)window;

    switch (m_event.type) {

    case ClientMessage:
      if ((Atom)m_event.xclient.data.l[0] == m_wmDeleteMessage) {
        exit();
      }
      break;
    case KeyPress:
      if (XK_Escape == XLookupKeysym(&m_event.xkey, 0)) {
        exit();
      }
      break;
    case Expose:
      redraw();
      XMapWindow(m_display, m_window);

      for (auto w_ : m_layout->getWidgets()) {
        w_->updateSizeAndPos();
        XMapWindow(m_display, w_->id());
      }
      break;
    case ButtonPress:
    case ButtonRelease:

      for (auto w_ : m_layout->getWidgets()) {
        w_->setFocus(w_->id() == m_event.xany.window);
      }
      break;
      /*
    case ButtonPress:
      XUngrabPointer(m_display, CurrentTime);
      break;
    case EnterNotify:
      for (auto w_ : m_widgets) {
        if (w_->isVisible()) {
          XMapWindow(m_display, w_->id());
        } else {
          XUnmapWindow(m_display, w_->id());
        }
      }
      XFlush(m_display);
      break;
    case LeaveNotify:
      // XSetWindowBackground(m_display, W[window].id, 0xFFFFFF);
      // XClearWindow(m_display, W[window].id);
      break;
      */
    }

    for (auto w_ : m_layout->getWidgets()) {
      if (w_->hasFocus()) {
        w_->handleEvent(m_event);
      }
    }
    redraw();
  }

  XUnmapWindow(m_display, m_window);
  XDestroyWindow(m_display, m_window);
  XCloseDisplay(m_display);
}
