#include "../headers/listview.h"
#include "../headers/application.h"
#include "../headers/label.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <algorithm>
#include <vector>

ListView::ListView() {
  m_display = Application::instance()->display();
  auto pw = Application::instance()->window();

  XSetWindowAttributes attr = {
      .background_pixel = 0xFFFFFF,
      .event_mask = ExposureMask | EnterWindowMask | LeaveWindowMask | ButtonPressMask,
  };

  m_window = Widget::createWindow(m_display, {0, 0, 1, 1}, attr, pw);
  m_painter = new Painter(m_display, m_window);
  m_rows = {
      "Item 1", "Item 2", "Item 3", "Item 4",  "Item 5",  "Item 6",
      "Item 7", "Item 8", "Item 9", "Item 10", "Item 11", "Item 12",
  };
}

ListView::~ListView() {
  XDestroyWindow(m_display, m_window);
}

ListView::ListView(std::vector<std::string> rows) : ListView() {
  m_rows = rows;
}

void ListView::removeItem(int index) {
  m_rows.erase(m_rows.begin() + index);
}

void ListView::addItem(std::string item) {
  m_rows.push_back(item);
}

void ListView::addItemAt(int index, std::string item) {
  m_rows.insert(m_rows.begin() + index, item);
}

void ListView::recalculateItems() {
  if (m_rows.size() == m_rects.size())
    return;
  m_rects.clear();
  for (int i = 0; i < (int)m_rows.size(); ++i) {
    int ypos = m_itemHeight * i;
    m_rects.push_back({.x = 1, .y = ypos + 1, .w = m_rect.w - 2, .h = (int)m_itemHeight - 2});
  }
}

unsigned int ListView::findClosestIndex(int position) {
  return position / m_itemHeight;
}

bool ListView::keyPressEvent(KeySym key, std::string) {
  if (!Application::instance()->isFocused(id()))
    return false;

  switch (key) {
  case XK_Up:
    if (m_selectedItem > 0)
      m_selectedItem--;
    return true;
  case XK_Down:
    if (m_selectedItem < m_rows.size())
      m_selectedItem++;
    return true;
  default:
    return false;
  }

  return false;
}

bool ListView::mousePressEvent(XButtonEvent &e, MouseButton btn) {
  if (Application::instance()->isFocused(id()))
    return false;
  if (btn != MouseButton::Left)
    return false;
  m_selectedItem = findClosestIndex(e.y);

  return true;
}

void ListView::paintEvent(XEvent &e) {
  recalculateItems();
  m_painter->clear();

  unsigned int maxItems = std::min(m_rect.h / (double)m_itemHeight, (double)m_rects.size());

  m_painter->setForeground(0x000000);
  m_painter->drawRect(0, 0, m_rect.w - 1, m_rect.h - 1);

  unsigned long sColor = 0xAAAAAA;
  if (Application::instance()->isFocused(id())) {
    sColor = 0xFF0000;
  }

  for (unsigned int i = 0; i < maxItems; ++i) {
    Rect r = m_rects[i];
    m_painter->setForeground(m_selectedItem == i ? sColor : 0xFFFFFF);
    m_painter->fillRect(r.x, r.y, r.w, r.h);
    m_painter->setForeground(m_selectedItem == i ? 0xFFFFFF : 0x000000);
    m_painter->drawString(m_rows[i].c_str(), m_rect.x - 5, r.y + r.h / 2);
  }

  m_painter->swapBuffers();
}
