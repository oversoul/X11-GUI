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

  bool scrollUp();
  bool scrollDown();
  void removeItem(int);
  void addItem(std::string);
  void addItemAt(int, std::string);

  void selectClosestIndex(int);

  void paintEvent(XEvent &) override;
  bool keyPressEvent(KeyEvent) override;
  bool mousePressEvent(XButtonEvent &, MouseButton) override;
  bool mouseScrollEvent(XButtonEvent &, MouseWheelDirection) override;

private:
  void recalculateItems();
  uint m_areas = 0;
  uint m_scroll = 0;
  uint m_itemHeight = 35;
  uint m_selectedItem = 0;
  std::vector<Rect> m_rects;
  std::vector<std::string> m_rows;
};
