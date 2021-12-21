#include "../headers/painter.h"
#include "../headers/application.h"
#include <cstring>

Painter::Painter(Application *app) : m_app(app) {}

Painter::~Painter() {}

void Painter::drawString(const char *text, int x, int y) { //
  XDrawString(m_app->display(), m_app->window(), m_app->gc(), x, y, text, strlen(text));
}
