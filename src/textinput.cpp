#include "../include/textinput.h"
#include <cstdio>

TextInput::TextInput() {
  newWindow();
}

TextInput::TextInput(std::string value) : TextInput() {
  m_cursor = value.length();
  setValue(value);
}

TextInput::~TextInput() {
}

bool isSpecialButton(KeySym key) {
  return (
       key == XK_Tab
    || key == XK_Delete 
    || key == XK_Return
    || key == XK_Escape
    || key == XK_Shift_L
    || key == XK_Caps_Lock
  );
}

bool TextInput::handleHotKeys(KeyEvent e) {
  if (e.mod == KeyModifier::Ctrl && e.key == XK_v) {
    auto v = m_server->getClipboard();  
    m_value.insert(m_cursor, v);
    m_cursor += v.length();
    return true;
  }
  return false;
}

bool TextInput::keyPressEvent(KeyEvent e) {
  if (!isFocused())
    return false;

  if (e.mod != KeyModifier::Non) {
    return handleHotKeys(e);
  }

  if (e.key == XK_Tab) {
    return false;
  }

  if (e.key == XK_BackSpace) {
    if (m_cursor > 0) {
      m_value.erase(m_cursor - 1, 1);
      m_cursor--;
    }
    return true;
  }

  if (e.key == XK_Left) {
    if (m_cursor > 0) {
      m_cursor--;
    }
    return true;
  }

  if (e.key == XK_Right) {
    if (m_cursor < m_value.length()) {
      m_cursor++;
    }
    return true;
  }

  // if (isSpecialButton(e.key))
  if (e.text == "")
    return false;

  m_value.insert(m_cursor, e.text);
  m_cursor++;
  return true;
}

void TextInput::paintEvent() {
  m_painter->clear(m_bgColor, m_rect);
  auto pad = 20;
  m_painter->drawString(m_value.c_str(), pad, m_rect.h / 2);

  m_painter->setForeground(isFocused() ? "#ff0000" : "#000000");
  m_painter->drawRect({0, 0, m_rect.w - 1, m_rect.h - 1});

  if (isFocused()) {
    auto pos = m_painter->textWidth(m_value.substr(0, m_cursor).c_str()) + pad;
    m_painter->drawLine(pos, pad, pos, m_rect.h - pad);
  }

  m_painter->swapBuffers();
}

void TextInput::setValue(std::string value) {
  m_value = value;
}
