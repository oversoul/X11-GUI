#include <iostream>

#include "../headers/application.h"
#include "../headers/button.h"
#include "../headers/label.h"
#include "../headers/layout.h"
#include "../headers/listview.h"
#include "../headers/textinput.h"

int main(void) {
  Application *app = new Application("Howdy!");

  Label la("First Item:");
  ListView lv;
  TextInput te("Hello World input");

  Button btn("Close");
  btn.setOnClick([app] { //
    app->triggerExit();
  });

  VLayout layout;
  layout.addWidget(&la);
  layout.addWidget(&te);
  layout.addWidget(&lv, 8);
  layout.addWidget(&btn);

  layout.setFullSize(20);
  layout.setSpaceBetween(10);
  app->setLayout(layout);

  app->exec();
  return 0;
}
