#include "listview.h"

ListView::ListView() {
  newWindow();

  m_rows = {
      "Item 1", "Item 2", "Item 3", "Item 4",  "Item 5",  "Item 6",
      "Item 7", "Item 8", "Item 9", "Item 10", "Item 11", "Item 12",
  };
}

ListView::~ListView() {
  m_rows.clear();
  m_rects.clear();
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
  uint width = m_rect.w - 2;
  uint height = m_itemHeight - 2;

  for (uint i = 0; i < m_rows.size(); ++i) {
    uint ypos = m_itemHeight * i;
    m_rects.push_back({1, ypos + 1, width, height});
  }
}

bool ListView::keyPressEvent(KeyEvent ke) {
  if (!isFocused())
    return false;

  if (ke.key == XK_Up) {
    if (m_selectedItem > 0) {
      m_selectedItem--;
      scrollDown(m_itemHeight);
    }
    return true;
  }

  if (ke.key == XK_Down) {
    if (m_selectedItem < m_rows.size() - 1) {
      m_selectedItem++;
      scrollUp(m_itemHeight);
    }
    return true;
  }

  return false;
}

bool ListView::mousePressEvent(MouseEvent e) {
  if (!isFocused() || e.button != MouseButton::Left)
    return false;

  uint index = (e.y + m_scroll) / m_itemHeight;
  m_selectedItem = index;

  return true;
}

bool ListView::scrollUp(uint amount) {
  // total
  double max = (m_rows.size() * m_itemHeight) - m_rect.h;

  if (m_scroll < max) {
    m_scroll += amount;
    if (m_scroll > max)
      m_scroll = max;
  }
  return true;
}

bool ListView::scrollDown(uint amount) {
  if (m_scroll > 0) {
    m_scroll -= amount;
    if (m_scroll < 0)
      m_scroll = 0;
  }
  return true;
}

bool ListView::mouseScrollEvent(MouseEvent e) {
  if (!isFocused())
    return false;

  if (e.direction == WheelDirection::Down)
    return scrollUp();

  if (e.direction == WheelDirection::Up)
    return scrollDown();

  return false;
}

void ListView::paintEvent() {
  recalculateItems();
  m_painter->clear(m_bgColor, m_rect);

  const char *sColor = isFocused() ? "#FF0000" : "#AAAAAA";

  for (uint i = 0; i < m_rows.size(); ++i) {
    Rect r = m_rects[i];
    r.y -= m_scroll;
    auto isCurrent = m_selectedItem == i;
    m_painter->setForeground(isCurrent ? sColor : "#FFFFFF");
    m_painter->fillRect(r);
    m_painter->drawString(m_rows[i].c_str(), m_rect.x - 5, r.y + r.h / 2, isCurrent ? "#FFFFFF" : "#000000");
  }

  m_painter->setForeground("#000000");
  m_painter->drawRect({0, 0, m_rect.w - 1, m_rect.h - 1});

  m_painter->swapBuffers();
}
