#include <iostream>

#include "../headers/application.h"
#include "../headers/label.h"
#include "../headers/layout.h"
#include "../headers/textinput.h"

int main(void) {
  Application *app = new Application("Howdy!", 640, 480);

  TextInput te;
  Label la;
  la.setValue("First Item:");
  te.setValue("Hello world");

  VLayout layout;
  layout.setRect({.x = 10, .y = 10, .w = 620, .h = 460});
  layout.setSpaceBetween(20);

  layout.addWidget(&la, 1);
  layout.addWidget(&te, 1);

  app->setLayout(layout);

  app->exec();
  return 0;
}
