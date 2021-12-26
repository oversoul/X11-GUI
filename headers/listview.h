#pragma once

#include "painter.h"
#include "widget.h"
#include <X11/Xlib.h>
#include <string>
#include <vector>

class ListView : public Widget {
public:
  ListView();
  ~ListView();
  ListView(std::vector<std::string>);

  void removeItem(int);
  void addItem(std::string);
  void addItemAt(int, std::string);

  unsigned int findClosestIndex(int);

  void paintEvent(XEvent &);
  bool mousePressEvent(XButtonEvent &, MouseButton);

  const Window id() const { return m_window; }

private:
  void recalculateItems();
  unsigned int m_itemHeight = 35;
  unsigned int m_selectedItem = 0;
  bool m_shouldRepaint = false;
  Painter *m_painter = nullptr;
  std::vector<Rect> m_rects;
  std::vector<std::string> m_rows;
};
