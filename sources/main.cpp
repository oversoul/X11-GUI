#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <iostream>

#include "../headers/application.h"
#include "../headers/label.h"
#include "../headers/textinput.h"

int main(void) {
  Application *app = new Application("Howdy!", 640, 480);

  TextInput te, tee;
  Label la;
  la.setValue("First Item:");
  te.setValue("Hello world");
  tee.setValue("Hello world");

  la.setRect({.x = 10, .y = 10, .w = 620, .h = 30});
  te.setRect({.x = 10, .y = 50, .w = 620, .h = 30});
  tee.setRect({.x = 10, .y = 90, .w = 620, .h = 30});

  app->addWidget(&la);
  app->addWidget(&te);
  app->addWidget(&tee);

  app->exec();
  return 0;
}
