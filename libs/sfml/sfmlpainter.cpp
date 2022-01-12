#include "sfmlpainter.h"
#include "color.h"
#include "typedefs.h"

SfmlPainter::SfmlPainter() {
}

SfmlPainter::~SfmlPainter() {
}

void SfmlPainter::drawString(const char *text, int x, int y, std::string color) {
}

void SfmlPainter::drawLine(int x1, int y1, int x2, int y2) {
}

void SfmlPainter::drawCircle(Rect r) {
}

void SfmlPainter::fillCircle(Rect r) {
}

void SfmlPainter::drawPoint(int x, int y) {
}

void SfmlPainter::clear(std::string color, Rect r) {
}

void SfmlPainter::drawRect(Rect r) {
}

void SfmlPainter::fillRect(Rect r) {
}

void SfmlPainter::setForeground(std::string color) {
}

uint SfmlPainter::textWidth(const char *text) {
  return 0;
}

void SfmlPainter::swapBuffers() {
}
