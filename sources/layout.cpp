#include "../headers/layout.h"

void Layout::setRect(Rect r) { //
  m_rect = r;
}

const Rect Layout::getRect() const { //
  return m_rect;
}

void Layout::setSpaceBetween(unsigned int sb) { //
  m_spaceBetween = sb;
}

void Layout::addWidget(Widget *widget, unsigned int w) {
  assert(w > 0 && "Weight should at least be 1");
  m_widgets.push_back(widget);
  m_weights.push_back(w);
}

VLayout::VLayout() {}
VLayout::~VLayout() {}
void VLayout::updatePosition() {
  if (m_widgets.empty() || m_widgets.size() != m_weights.size())
    return;
  if (m_rect.h <= 1)
    return;

  unsigned int totalHeight = m_rect.h - (m_widgets.size() - 1) * m_spaceBetween;

  int ypos = 0;
  int weightSum = 0;
  for (auto &w : m_weights) {
    weightSum += w;
  }

  int height = totalHeight / weightSum;

  for (size_t i = 0; i < m_widgets.size(); ++i) {
    int h = height * m_weights[i];
    m_widgets[i]->setRect({.x = m_rect.x, .y = ypos, .w = m_rect.w, .h = h});
    ypos += h + m_spaceBetween;
  }
}

// ------------------

HLayout::HLayout() {}
HLayout::~HLayout() {}
void HLayout::updatePosition() {
  if (m_widgets.empty() || m_widgets.size() != m_weights.size())
    return;
  if (m_rect.h <= 1)
    return;

  unsigned int totalWidth = m_rect.w - (m_widgets.size() - 1) * m_spaceBetween;

  int xpos = 0;
  int weightSum = 0;
  for (auto &w : m_weights) {
    weightSum += w;
  }

  int width = totalWidth / weightSum;

  for (size_t i = 0; i < m_widgets.size(); ++i) {
    int w = width * m_weights[i];
    m_widgets[i]->setRect({.x = xpos, .y = m_rect.y, .w = w, .h = m_rect.h});
    xpos += w + m_spaceBetween;
  }
}
