#pragma once
#include "rect.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

Display *openDisplay();
void loadXdbeExtension(Display *dpy);
int getScreens(Display *dpy, int use_anchors, int *left_x, int *right_x, int *top_y, int *bottom_y);
void getMonitorSize(Display *dpy, uint *width, uint *height);
Window createWindow(Display *dpy, XSetWindowAttributes attr, Window p);

void setWindowType(Display *dpy, Window w, std::string type);
Atom addWindowState(Display *dpy, Window w, std::string type);
Atom changeWMprop(Display *dpy, Window w, std::string property, const char *data, Bool overwrite);

unsigned long stringToKeysym(const char *key);
