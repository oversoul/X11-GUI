#include "../headers/layout.h"
#include "../headers/application.h"

void Layout::setRect(Rect r) {
  m_rect = r;
}

const Rect Layout::getRect() const {
  return m_rect;
}

void Layout::setSpaceBetween(unsigned int sb) {
  m_spaceBetween = sb;
}

void Layout::setFullSize(unsigned int padding) {
  m_rect = {
      .x = padding,
      .y = padding,
      .w = Application::instance()->width() - padding * 2,
      .h = Application::instance()->height() - padding,
  };
}

void Layout::addWidget(Widget *widget, unsigned int w) {
  assert(w > 0 && "Weight should at least be 1\n");
  m_widgets.push_back(widget);
  m_weights.push_back(w);
}

VLayout::VLayout() {
}

VLayout::~VLayout() {
}

void VLayout::updatePosition() {
  if (m_widgets.empty() || m_widgets.size() != m_weights.size())
    return;
  if (m_rect.h <= 1)
    return;

  unsigned int totalHeight = m_rect.h - (m_widgets.size() - 1) * m_spaceBetween - m_rect.y;

  unsigned int ypos = m_rect.y;
  unsigned int weightSum = 0;
  for (auto &w : m_weights) {
    weightSum += w;
  }

  unsigned int height = totalHeight / weightSum;
  for (size_t i = 0; i < m_widgets.size(); ++i) {
    unsigned int h = height * m_weights[i];
    m_widgets[i]->setRect({m_rect.x, ypos, m_rect.w, h});
    ypos += h + m_spaceBetween;
  }
}

// ------------------

HLayout::HLayout() {
}

HLayout::~HLayout() {
}

void HLayout::updatePosition() {
  if (m_widgets.empty() || m_widgets.size() != m_weights.size())
    return;
  if (m_rect.h <= 1)
    return;

  unsigned int totalWidth = m_rect.w - (m_widgets.size() - 1) * m_spaceBetween - m_rect.x;

  unsigned int xpos = m_rect.x;
  unsigned int weightSum = 0;
  for (auto &w : m_weights) {
    weightSum += w;
  }

  unsigned int width = totalWidth / weightSum;

  for (size_t i = 0; i < m_widgets.size(); ++i) {
    unsigned int w = width * m_weights[i];
    m_widgets[i]->setRect({xpos, m_rect.y, w, m_rect.h});
    xpos += w + m_spaceBetween;
  }
}
