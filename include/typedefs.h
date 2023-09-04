#pragma once
#include <iostream>

enum class KeyModifier { Non, Ctrl, Shift };

const char* mod_to_string(KeyModifier mod);

enum class MouseButton { Unknown, Left, Middle, Right, Scroll };
enum class WheelDirection { Up, Down, Unknown };

typedef struct {
  KeyModifier mod;
  unsigned long key;
  std::string text;
} KeyEvent;

typedef struct {
  int x, y;
  bool isScroll;
  MouseButton button;
  WheelDirection direction;
} MouseEvent;

void print_modifiers(uint mask);