#pragma once
#include "rect.h"
#include <iostream>

class Painter {
public:
  virtual ~Painter();
  virtual void drawPoint(int x, int y);
  virtual void drawRect(Rect r);
  virtual void drawLine(int x1, int y1, int x2, int y2);
  virtual void fillRect(Rect r);
  virtual void drawCircle(Rect r);
  virtual void fillCircle(Rect r);
  virtual uint textWidth(const char *text);
  virtual void drawString(const char *text, int x, int y, std::string color = "#000000");
  virtual void setForeground(std::string color);
  virtual void clear(std::string color, Rect r);
  virtual void swapBuffers();
};
