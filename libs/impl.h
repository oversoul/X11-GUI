#pragma once

#if S_IMPL == xlib
#include "xlib.h"
#elif S_IMPL == xcb
#include "xcb.h"
#else
#endif

WindowServer *newServer(ServerType type);
