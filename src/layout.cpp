#include "../include/layout.h"
#include "../include/application.h"

void Layout::setRect(Rect r) {
  m_rect = r;
}

const Rect Layout::getRect() const {
  return m_rect;
}

void Layout::setSpaceBetween(uint sb) {
  m_spaceBetween = sb;
}

void Layout::setFullSize(uint vert, uint hor) {
  m_rect = {
      .x = hor,
      .y = vert,
      .w = Application::instance()->width() - hor * 2,
      .h = Application::instance()->height() - vert,
  };
}

const Widget *Layout::getFirstWidget() const {
  return (m_widgets.size() > 0) ? m_widgets[0] : nullptr;
}

void Layout::addWidget(Widget *widget, uint w) {
  assert(w > 0 && "Weight should at least be 1\n");
  m_widgets.push_back(widget);
  m_weights.push_back(w);
}

void VLayout::updatePosition() {
  if ((m_widgets.empty() || m_widgets.size() != m_weights.size()) || m_rect.h <= 1)
    return;

  uint totalHeight = m_rect.h - (m_widgets.size() - 1) * m_spaceBetween - m_rect.y;

  uint ypos = m_rect.y;
  uint weightSum = 0;
  for (auto &w : m_weights) {
    weightSum += w;
  }

  uint height = totalHeight / weightSum;
  for (size_t i = 0; i < m_widgets.size(); ++i) {
    uint h = height * m_weights[i];
    m_widgets[i]->setRect({m_rect.x, ypos, m_rect.w, h});
    ypos += h + m_spaceBetween;
  }
}

// ------------------
void HLayout::updatePosition() {
  if ((m_widgets.empty() || m_widgets.size() != m_weights.size()) || m_rect.h <= 1)
    return;

  uint totalWidth = m_rect.w - (m_widgets.size() - 1) * m_spaceBetween - m_rect.x;

  uint xpos = m_rect.x;
  uint weightSum = 0;
  for (auto &w : m_weights) {
    weightSum += w;
  }

  uint width = totalWidth / weightSum;

  for (size_t i = 0; i < m_widgets.size(); ++i) {
    uint w = width * m_weights[i];
    m_widgets[i]->setRect({xpos, m_rect.y, w, m_rect.h});
    xpos += w + m_spaceBetween;
  }
}
