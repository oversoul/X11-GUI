#include "../headers/textinput.h"
#include "../headers/application.h"
#include "../headers/widget.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdio>

TextInput::TextInput() {
  m_display = Application::instance()->display();
  auto pw = Application::instance()->window();

  Rect r = {0, 0, 1, 1};

  XSetWindowAttributes attr = {
      .background_pixel = m_bgColor,
      .event_mask =
          ButtonPressMask | ButtonReleaseMask | ExposureMask | EnterWindowMask | LeaveWindowMask | KeyPressMask,
  };

  m_window = Widget::createWindow(m_display, r, attr, pw);
  m_painter = new Painter(m_display, m_window);
}
TextInput::TextInput(std::string value) : TextInput() { //
  setValue(value);
}

TextInput::~TextInput() {
  //
}

bool TextInput::mousePressEvent(XButtonEvent &) { //
  return false;
}

bool TextInput::keyPressEvent(XKeyEvent &e) {
  if (!hasFocus())
    return false;
  KeySym key;
  char text[255];

  if (XLookupString(&e, text, 255, &key, 0) == 1) {
    if (key == XK_BackSpace) {
      if (m_value.size() > 0)
        m_value.pop_back();
      return true;
    }

    if (key == XK_Tab || key == XK_BackSpace)
      return false;

    m_value += text;
    return true;
  }
  return false;
}

bool TextInput::keyReleaseEvent(XKeyEvent &e) { //
  return false;
}

const bool TextInput::hasFocus() const { //
  return Application::instance()->focusedWindow() == id();
}

void TextInput::paintEvent(XEvent &) {
  m_painter->clear();
  setBackground(m_bgColor);

  m_painter->setForeground(Application::instance()->focusedWindow() == id() ? 0xff0000 : 0x000000);
  m_painter->drawRect(0, 0, m_rect.w - 1, m_rect.h - 1);

  m_painter->setForeground(0x000000);
  m_painter->drawString(m_value.c_str(), 5, m_rect.h / 2);
}

void TextInput::setValue(std::string value) { //
  m_value = value;
}
