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
  delete m_painter;
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
  unsigned int width = m_rect.w - 2;
  unsigned int height = m_itemHeight - 2;
  m_areas = m_rect.h / m_itemHeight;

  for (unsigned int i = 0; i < m_areas; ++i) {
    unsigned int ypos = m_itemHeight * i;
    m_rects.push_back({1, ypos + 1, width, height});
  }
}

void ListView::selectClosestIndex(int position) {
  unsigned int index = position / m_itemHeight;
  if (index < m_areas)
    m_selectedItem = index;
}

bool ListView::keyPressEvent(KeySym key, std::string) {
  if (!isFocused())
    return false;

  if (key == XK_Up) {
    if (m_selectedItem > 0) {
      m_selectedItem--;
    } else {
      scrollDown();
    }
    return true;
  }

  if (key == XK_Down) {
    if (m_selectedItem < m_areas - 1) {
      m_selectedItem++;
    } else {
      scrollUp();
    }
    return true;
  }

  return false;
}

bool ListView::mousePressEvent(XButtonEvent &e, MouseButton btn) {
  if (!isFocused())
    return false;
  if (btn != MouseButton::Left)
    return false;

  selectClosestIndex(e.y);

  // std::cout << "selected: " << m_rows[m_selectedItem + m_scroll] << std::endl;

  return true;
}

bool ListView::scrollUp() {
  if ((m_scroll < m_rows.size() - m_areas) && m_rows.size() > m_areas)
    ++m_scroll;
  return true;
}

bool ListView::scrollDown() {
  if (m_scroll > 0)
    --m_scroll;
  return true;
}

bool ListView::mouseScrollEvent(XButtonEvent &, MouseWheelDirection dir) {
  if (!isFocused())
    return false;

  if (dir == MouseWheelDirection::Down)
    return scrollUp();

  if (dir == MouseWheelDirection::Up)
    return scrollDown();

  return false;
}

void ListView::paintEvent(XEvent &e) {
  recalculateItems();
  m_painter->clear();

  m_painter->setForeground(0x000000);
  m_painter->drawRect(0, 0, m_rect.w - 1, m_rect.h - 1);

  unsigned long sColor = isFocused() ? 0xFF0000 : 0xAAAAAA;

  for (unsigned int i = 0; i < m_areas; ++i) {
    if (i + m_scroll == m_rows.size())
      break;

    Rect r = m_rects[i];
    m_painter->setForeground(m_selectedItem == i ? sColor : 0xFFFFFF);
    m_painter->fillRect(r.x, r.y, r.w, r.h);
    m_painter->setForeground(m_selectedItem == i ? 0xFFFFFF : 0x000000);
    m_painter->drawString(m_rows[i + m_scroll].c_str(), m_rect.x - 5, r.y + r.h / 2);
  }

  m_painter->swapBuffers();
}
