#include "painter.h"

Painter::~Painter() {
}

void Painter::drawString(const char *text, int x, int y, std::string color) {
}

void Painter::drawLine(int x1, int y1, int x2, int y2) {
}

void Painter::drawCircle(Rect r) {
}

void Painter::fillCircle(Rect r) {
}

void Painter::drawPoint(int x, int y) {
}

void Painter::clear(std::string color, Rect r) {
}

void Painter::drawRect(Rect r) {
}

void Painter::fillRect(Rect r) {
}

void Painter::setForeground(std::string color) {
}

uint Painter::textWidth(const char *text) {
  return -1;
}

void Painter::swapBuffers() {
}
