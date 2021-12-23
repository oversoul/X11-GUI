#pragma once

#include "painter.h"
#include "widget.h"
#include <X11/Xlib.h>
#include <string>
#include <vector>

class ListView : public Widget {
  //
public:
  ListView();
  ~ListView();
  ListView(std::vector<std::string> rows);

  void paintEvent(XEvent &);

  const Window id() const { return m_window; }

private:
  bool m_shouldRepaint = false;
  Painter *m_painter = nullptr;
  std::vector<std::string> m_rows;
};
