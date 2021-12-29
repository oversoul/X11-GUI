#pragma once
#include <iostream>

typedef struct RectStruct {
  unsigned int x, y, w, h;

  void print_r() {
    std::cout << "x: " << x << " y: " << y << " w: " << w << " h: " << h << std::endl;
  }

  bool inRegion(unsigned int mx, unsigned int my, RectStruct r) {
    bool inX = x >= r.x && x <= r.h;
    bool inY = y >= r.y && y <= r.w;
    return inX && inY;
  }

} Rect;
