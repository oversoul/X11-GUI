#pragma once

#if S_IMPL == xlib
#include "../libs/xlib/xlib.h"
#elif S_IMPL == xcb
#include "../libs/xcb/xcb.h"
#else
#endif

WindowServer *newServer();
