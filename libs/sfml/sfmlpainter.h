#pragma once
#include "font.h"
#include "painter.h"
#include "rect.h"
#include <iostream>

class SfmlPainter : public Painter {
public:
  SfmlPainter();
  ~SfmlPainter();

  void swapBuffers() override;
  void drawRect(Rect r) override;
  void fillRect(Rect r) override;
  void drawCircle(Rect r) override;
  void fillCircle(Rect r) override;
  void drawPoint(int x, int y) override;
  uint textWidth(const char *text) override;
  void setForeground(std::string color) override;
  void clear(std::string color, Rect r) override;
  void drawLine(int x1, int y1, int x2, int y2) override;
  void drawString(const char *text, int x, int y, std::string color = "#000000") override;

private:
};
