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

  bool scrollUp();
  bool scrollDown();

  unsigned int findClosestIndex(int);

  void paintEvent(XEvent &);

  bool keyPressEvent(KeySym, std::string);
  bool mousePressEvent(XButtonEvent &, MouseButton);
  bool mouseScrollEvent(XButtonEvent &, MouseWheelDirection);

  const Window id() const {
    return m_window;
  }

private:
  void recalculateItems();
  unsigned int m_scroll = 0;
  unsigned int m_areas = 0;
  unsigned int m_itemHeight = 35;
  unsigned int m_selectedItem = 0;
  bool m_shouldRepaint = false;
  Painter *m_painter = nullptr;
  std::vector<Rect> m_rects;
  std::vector<std::string> m_rows;
};
