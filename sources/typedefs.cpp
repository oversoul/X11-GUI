#include "../headers/typedefs.h"
#include <iostream>

int getScreens(Display *dpy, int use_anchors, int *left_x, int *right_x, int *top_y, int *bottom_y) {
  // Get currently focused window
  Window win = -1;
  int focus_status;
  XGetInputFocus(dpy, &win, &focus_status);

  if (win == PointerRoot || win == None)
    return -1;

  XWindowAttributes win_attr;
  XRRScreenResources *screen_res = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
  XGetWindowAttributes(dpy, win, &win_attr);

  int det_x = 0, det_y = 0, nmonitors = 0;

  XRRGetMonitors(dpy, win, 1, &nmonitors);

  for (int i = 0; i < nmonitors; i++) {
    XRRCrtcInfo *screen_info = XRRGetCrtcInfo(dpy, screen_res, screen_res->crtcs[i]);
    // option flag for using the "anchor" (top left corner)  of a window to determine what screen it belongs to
    if (use_anchors == 1) {
      det_x = win_attr.x;
      det_y = win_attr.y;
      // Use the center of the window to determine what screen it's on
    } else {
      det_x = win_attr.x + ((win_attr.width) / 2);
      det_y = win_attr.y + ((win_attr.height) / 2);
    }

    // If the window is on the ith screen in the x
    if (det_x >= screen_info->x && det_x < (int)(screen_info->x + screen_info->width)) {
      // If the window is on the ith screen in the y
      if (det_y >= screen_info->y && det_y < (int)(screen_info->y + screen_info->height)) {
        *left_x = screen_info->x;
        *right_x = screen_info->x + screen_info->width;
        *top_y = screen_info->y;
        *bottom_y = screen_info->y + screen_info->height;
        return 0;
      }
    }
  }

  // If the function has not returned yet, then it could not find a screen on which 'win' resides.
  return -1;
}

void getMonitorSize(Display *dpy, unsigned int *width, unsigned int *height) {
  int left_x = 0, right_x = 0, top_y = 0, bottom_y = 0;
  if (getScreens(dpy, 1, &left_x, &right_x, &top_y, &bottom_y) < 0) {
    *width = XDisplayWidth(dpy, XDefaultScreen(dpy));
    *height = XDisplayHeight(dpy, XDefaultScreen(dpy));
  } else {
    *width = right_x;
    *height = bottom_y;
  }
}
