#pragma once

#include <iostream>

class Application;

class Painter {
public:
  Painter(Application *app);
  ~Painter();
  void drawString(const char *text, int x, int y);

private:
  Application *m_app = nullptr;
};
