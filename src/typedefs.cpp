#include "typedefs.h"
#include <iostream>


const char* mod_to_string(KeyModifier mod) {
  switch(mod) {
    case KeyModifier::Ctrl:
      return "ctrl";
    case KeyModifier::Shift:
      return "shift";
    default:
      return "";
  }
}


// print key modifier...
void print_modifiers(uint mask) {
  const char *MODIFIERS[] = {"Shift", "Lock", "Ctrl", "Alt", "Mod2", "Mod3", "Mod4", "Mod5"};
  for (const char **modifier = MODIFIERS; mask; mask >>= 1, ++modifier) {
    if (mask & 1) {
      printf("Modifier mask: %s\n", *modifier);
    }
  }
}

/*
static int error_handler(Display *d, XErrorEvent *e) {
  std::cerr << "Error minor code: " << e->minor_code << std::endl;
  std::cerr << "Error code: " << e->error_code << std::endl;
  return 0;
}
*/

/*
Atom changeWMprop(Display *dpy, Window w, std::string property, const char *data, Bool overwrite) {
  Atom wmatom = XInternAtom(dpy, data, False);
  Atom atom = XInternAtom(dpy, property.c_str(), False);
  XChangeProperty(dpy, w, atom, XA_ATOM, 32, overwrite ? PropModeReplace : PropModeAppend, (unsigned char *)&wmatom, 1);
  return wmatom;
}

Atom addWindowState(Display *dpy, Window w, std::string type) {
  std::string prop = "_NET_WM_STATE_" + type;
  return changeWMprop(dpy, w, "_NET_WM_STATE", prop.c_str(), False);
}

void setWindowType(Display *dpy, Window w, std::string type) {
  // called when creating window, preferably not called afterwards
  // 64 Window-Manager property text max. size
  std::string prop = "_NET_WM_WINDOW_TYPE_" + type;
  // printf("Setting window as type %s\n",type);
  changeWMprop(dpy, w, "_NET_WM_WINDOW_TYPE", prop.c_str(), True);
}
*/
