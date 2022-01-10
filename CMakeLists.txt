cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(xgui)


set(XGUI_HDR 
  xgui.h
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
  xgui.cpp
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

find_package(X11 REQUIRED)
if(NOT X11_Xrandr_FOUND)
  message(FATAL_ERROR "libXrandr not found")
endif()
if(NOT X11_Xext_FOUND)
  message(FATAL_ERROR "libXext not found")
endif()
if(NOT X11_Xft_FOUND)
  message(FATAL_ERROR "libXft not found")
endif()
find_package(Freetype REQUIRED)

if (server STREQUAL "xlib")
  include_directories(libs/xlib)
  set(XGUI_SERVER
    libs/xlib/xlib.cpp
    libs/xlib/xlibpainter.cpp
  )
  # add_subdirectory(libs/xlib)
  message(STATUS "server IS xlib")
elseif (server STREQUAL "xcb")
  message(STATUS "server IS xcb")
else()
  message(STATUS "none")
endif()

add_library(xgui SHARED
  ${XGUI_HDR}
  ${XGUI_SRCS}
  ${XGUI_SERVER}
)

include_directories(xgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(xgui ${X11_LIBRARIES})
target_include_directories(xgui PRIVATE ${X11_INCLUDE_DIR})

target_link_libraries(xgui ${X11_Xrandr_LIB})
target_include_directories(xgui PRIVATE ${X11_Xrandr_INCLUDE_PATH})

target_link_libraries(xgui ${X11_Xft_LIB})
target_include_directories(xgui PRIVATE ${X11_Xft_INCLUDE_PATH})

target_link_libraries(xgui ${FREETYPE_LIBRARIES})
target_include_directories(xgui PRIVATE ${FREETYPE_INCLUDE_DIRS})
