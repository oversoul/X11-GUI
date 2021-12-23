#include <iostream>

#include "../headers/application.h"
#include "../headers/label.h"
#include "../headers/layout.h"
#include "../headers/textinput.h"

int main(void) {
  Application *app = new Application("Howdy!", 640, 480);

  TextInput te, tee;
  Label la;
  la.setValue("First Item:");
  te.setValue("Hello world");
  tee.setValue("Hello world");

  /*
  app->addWidget(&la);
  app->addWidget(&te);
  app->addWidget(&tee);
  */

  VLayout layout;
  layout.setRect({.x = 10, .y = 10, .w = 620, .h = 460});
  layout.setSpaceBetween(20);

  layout.addWidget(&la);
  layout.addWidget(&te);
  layout.addWidget(&tee);

  app->setLayout(layout);

  app->exec();
  return 0;
}
