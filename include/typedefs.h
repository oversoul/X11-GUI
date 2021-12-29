#pragma once
#include "rect.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

void loadXdbeExtension(Display *dpy);
int getScreens(Display *dpy, int use_anchors, int *left_x, int *right_x, int *top_y, int *bottom_y);
void getMonitorSize(Display *dpy, unsigned int *width, unsigned int *height);
Window createWindow(Display *dpy, Rect r, XSetWindowAttributes attr, Window p = -1);
