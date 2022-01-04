#pragma once
#include <iostream>

typedef struct _Rect {
  uint x, y, w, h;

  void print_r() {
    std::cout << "x: " << x << " y: " << y << " w: " << w << " h: " << h << std::endl;
  }

  bool inRegion(uint mx, uint my, _Rect r) {
    bool inX = x >= r.x && x <= r.h;
    bool inY = y >= r.y && y <= r.w;
    return inX && inY;
  }

} Rect;
