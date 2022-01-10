cmake_minimum_required(VERSION 3.14.0)
set(XCB_FIND_QUIETLY ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(xgui)


set(XGUI_HDR 
  include/impl.h
  include/font.h
  include/color.h
  include/label.h
  include/button.h
  include/layout.h
  include/server.h
  include/widget.h
  include/painter.h
  include/typedefs.h
  include/application.h
  include/listview.h
  include/textinput.h
)

set(XGUI_SRCS 
  src/font.cpp
  src/color.cpp
  src/label.cpp
  src/button.cpp
  src/layout.cpp
  src/server.cpp
  src/widget.cpp
  src/painter.cpp
  src/typedefs.cpp
  src/listview.cpp
  src/textinput.cpp
  src/application.cpp
)

find_package(Freetype REQUIRED)

if (server STREQUAL "xlib")
  include_directories(libs/xlib)
  set(XGUI_SERVER libs/xlib/xlib.cpp libs/xlib/xlibpainter.cpp)
elseif (server STREQUAL "xcb")
  include_directories(libs/xcb)
  set(XGUI_SERVER libs/xcb/xcb.cpp libs/xcb/xcbpainter.cpp libs/xcb/xcbft.cpp)
else()
  message(FATAL_ERROR "Server not specifed (xlib, xcb).")
endif()

add_library(xgui SHARED
  ${XGUI_HDR}
  ${XGUI_SRCS}
  ${XGUI_SERVER}
)

# Linking
if (server STREQUAL "xlib")
  find_package(X11 REQUIRED)
  target_link_libraries(xgui ${X11_LIBRARIES})
  target_include_directories(xgui PRIVATE ${X11_INCLUDE_DIR})

  target_link_libraries(xgui ${X11_Xrandr_LIB})
  target_include_directories(xgui PRIVATE ${X11_Xrandr_INCLUDE_PATH})

  target_link_libraries(xgui ${X11_Xft_LIB})
  target_include_directories(xgui PRIVATE ${X11_Xft_INCLUDE_PATH})
elseif (server STREQUAL "xcb")
  find_package(XCB REQUIRED)
  target_link_libraries(xgui ${XCB_XCB_LIBRARY})
  target_link_libraries(xgui ${XCB_XRM_LIBRARY})
  target_link_libraries(xgui ${XCB_ICCCM_LIBRARY})
  target_link_libraries(xgui ${XCB_RENDER_LIBRARY})
  target_link_libraries(xgui ${XCB_KEYSYMS_LIBRARY})
  target_link_libraries(xgui ${XCB_RENDERUTIL_LIBRARY})
  target_include_directories(xgui PRIVATE ${XCB_INCLUDE_DIRS})

  find_package(Xkbcommon REQUIRED)
  target_link_libraries(xgui ${XKBCOMMON_LIBRARIES})

  find_package(Fontconfig REQUIRED)
  target_link_libraries(xgui ${Fontconfig_LIBRARIES})

  message(STATUS "hello: ${XKBCOMMON_INCLUDE_DIRS}")

else()
  message(FATAL_ERROR "Server not specifed (xlib, xcb).")
endif()

include_directories(xgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(xgui ${FREETYPE_LIBRARIES})
target_include_directories(xgui PRIVATE ${FREETYPE_INCLUDE_DIRS})
