#pragma once
#include "rect.h"
#include <iostream>

class Painter {
public:
  virtual ~Painter() {
  }
  virtual void swapBuffers() = 0;
  virtual void drawRect(Rect r) = 0;
  virtual void fillRect(Rect r) = 0;
  virtual void drawCircle(Rect r) = 0;
  virtual void fillCircle(Rect r) = 0;
  virtual void drawPoint(int x, int y) = 0;
  virtual uint textWidth(const char *text) = 0;
  virtual void setForeground(std::string color) = 0;
  virtual void clear(std::string color, Rect r) = 0;
  virtual void drawLine(int x1, int y1, int x2, int y2, int w = 1) = 0;
  virtual void drawString(const char *text, int x, int y, std::string color = "#000000") = 0;
};
