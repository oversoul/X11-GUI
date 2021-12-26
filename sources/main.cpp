#include <iostream>

#include "../headers/application.h"
#include "../headers/label.h"
#include "../headers/layout.h"
#include "../headers/listview.h"
#include "../headers/textinput.h"

int main(void) {
  Application *app = new Application("Howdy!", 640, 480);

  Label la("First Item:");
  TextInput te("Hello World input");
  ListView lv;

  VLayout layout;
  layout.addWidget(&la);
  layout.addWidget(&te);
  layout.addWidget(&lv, 8);

  layout.setFullSize(20);
  layout.setSpaceBetween(10);
  app->setLayout(layout);

  app->exec();
  return 0;
}
