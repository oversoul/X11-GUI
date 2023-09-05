#pragma once
#include <iostream>

typedef struct _Rect {
  uint x, y, w, h;

  void print_r() {
    std::cout << "x: " << x << " y: " << y << " w: " << w << " h: " << h << std::endl;
  }

  bool inRegion(_Rect r) {
    bool inX = x >= r.x && x <= r.h;
    bool inY = y >= r.y && y <= r.w;
    return inX && inY;
  }

  bool inRect(uint rx, uint ry) {
    bool inX = x >= rx && x <= rx + w;
    bool inY = y >= ry && y <= ry + h;
    return inX && inY;
  }

} Rect;
