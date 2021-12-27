#pragma once
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

int get_screens(Display *dpy, int use_anchors, int *left_x, int *right_x, int *top_y, int *bottom_y);

void getMonitorSize(Display *dpy, unsigned int *width, unsigned int *height);
