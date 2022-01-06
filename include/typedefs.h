#pragma once
#include <iostream>

typedef struct {
  unsigned long key;
  std::string text;
} KeyEvent;

enum class MouseButton { Unknown, Left, Middle, Right, Scroll };
enum class WheelDirection { Up, Down, Unknown };
typedef struct {
  int x, y;
  bool isScroll;
  MouseButton button;
  WheelDirection direction;
} MouseEvent;
