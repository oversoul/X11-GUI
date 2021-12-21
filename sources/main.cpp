#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

#include "../headers/application.h"
#include <iostream>

int main(void) {
  Application *app = new Application("Howdy!", 640, 480);

  KeySym key;
  char text[255];

  while (!app->shouldExit()) {

    auto event = app->fetchEvent();

    if (event.type == Expose && event.xexpose.count == 0) {
      app->redraw();
    }

    if (event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
      if (key == XK_Escape) {
        app->exit();
        break;
      }
      printf("You pressed the %c key!\n", text[0]);
    }
    if (event.type == ButtonPress) {
      int x = event.xbutton.x, y = event.xbutton.y;

      strcpy(text, "X is FUN!");
      app->setForeground(rand() % event.xbutton.x % 255);
      app->painter()->drawString(text, x, y);
    }
  }
}
