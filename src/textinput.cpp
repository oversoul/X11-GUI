#include "../include/textinput.h"
#include "../include/application.h"
#include "../include/typedefs.h"
#include <X11/keysym.h>
#include <cstdio>

TextInput::TextInput() {
  XSetWindowAttributes attr = {
      .background_pixel = m_bgColor,
      .event_mask =
          ButtonPressMask | ButtonReleaseMask | ExposureMask | EnterWindowMask | LeaveWindowMask | KeyPressMask,
  };

  newWindow(attr);
}

TextInput::TextInput(std::string value) : TextInput() {
  setValue(value);
}

TextInput::~TextInput() {
}

bool TextInput::keyPressEvent(KeyEvent ke) {
  if (!isFocused())
    return false;

  if (ke.key == XK_BackSpace) {
    if (m_value.size() > 0)
      m_value.pop_back();
    return true;
  }

  if (ke.key == XK_Tab || ke.key == XK_Delete || ke.key == XK_Return)
    return false;

  m_value += ke.text;
  return true;
}

void TextInput::paintEvent(XEvent &) {
  m_painter->clear(m_bgColor);
  m_painter->setForeground(isFocused() ? 0xff0000 : 0x000000);
  m_painter->drawRect({0, 0, m_rect.w - 1, m_rect.h - 1});
  m_painter->drawString(m_value.c_str(), 5, m_rect.h / 2);
  m_painter->swapBuffers();
}

void TextInput::setValue(std::string value) {
  m_value = value;
}
