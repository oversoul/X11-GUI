#include "../headers/listview.h"
#include "../headers/application.h"
#include "../headers/label.h"
#include <algorithm>
#include <vector>

ListView::ListView() {
  m_display = Application::instance()->display();
  auto pw = Application::instance()->window();

  Rect r = {0, 0, 1, 1};

  XSetWindowAttributes attr = {
      .background_pixel = 0xFFFFFF,
      .event_mask = ExposureMask | EnterWindowMask | LeaveWindowMask,
  };

  m_window = Widget::createWindow(m_display, r, attr, pw);
  m_painter = new Painter(m_display, m_window);
  m_rows = {
      "Item", "Item", "Item", "Item", "Item", "Item", "Item", "Item", "Item", "Item", "Item", "Item",
  };
}

ListView::~ListView() {}

ListView::ListView(std::vector<std::string> rows) : ListView() { //
  m_rows = rows;
}

void ListView::paintEvent(XEvent &e) {
  m_painter->clear();

  int height = 35;
  int maxItems = std::min(m_rect.h / (double)height, (double)m_rows.size());

  for (int i = 0; i < maxItems; ++i) {
    int ypos = height * i;
    m_painter->drawRect(0, ypos, m_rect.w - 1, height);
    m_painter->drawString(m_rows[i].c_str(), m_rect.x, ypos + height / 2);
  }
}
