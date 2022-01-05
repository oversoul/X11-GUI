#pragma once
#include "widget.h"
#include <string>
#include <vector>

#define SCROLL_AMOUNT 10

class ListView : public Widget {
public:
  ListView();
  ~ListView();
  ListView(std::vector<std::string>);

  bool scrollUp(uint amount = SCROLL_AMOUNT);
  bool scrollDown(uint amount = SCROLL_AMOUNT);
  void removeItem(int);
  void addItem(std::string);
  void addItemAt(int, std::string);

  void paintEvent() override;
  bool keyPressEvent(KeyEvent) override;
  bool mousePressEvent(MouseEvent) override;
  bool mouseScrollEvent(MouseEvent) override;

private:
  void recalculateItems();
  uint m_areas = 0;
  int m_scroll = 0;
  uint m_itemHeight = 35;
  uint m_selectedItem = 0;
  std::vector<Rect> m_rects;
  std::vector<std::string> m_rows;
};
