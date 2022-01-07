project(
  'xgui',
  'cpp',
  version : '1.0.0',
  default_options : ['warning_level=1']
)

project_description = 'Simpe X11 Gui System'

project_headers = [
  'xgui.h',
]

project_source_files = [
  'xgui.cpp',
  'src/application.cpp',
  'src/button.cpp',
  'src/widget.cpp',
  'src/painter.cpp',
  'src/color.cpp',
  'src/font.cpp',
  'src/label.cpp',
  'src/layout.cpp',
  'src/listview.cpp',
  'src/server.cpp',
  'src/textinput.cpp',
  'src/typedefs.cpp',
  'libs/xlib.cpp',
  'libs/xlibpainter.cpp',
]

build_args = [
]


# ===================================================================

# ======
# Target
# ======

public_headers = [
  include_directories('./'),
  include_directories('include'), 
  include_directories('libs')
]

deps = [ dependency('X11'), dependency('xext'), dependency('xrandr'), dependency('xft'), dependency('freetype2') ]

project_target = shared_library(
  meson.project_name(),
  project_source_files,
  include_directories : public_headers,
  dependencies: deps
)


# =======
# Project
# =======

# Make this library usable as a Meson subproject.
project_dep = declare_dependency(
  include_directories: public_headers,
  link_with : project_target
)

set_variable(meson.project_name() + '_dep', project_dep)

# Make this library usable from the system's
# package manager.
install_headers(project_headers, subdir : meson.project_name())

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : meson.project_name(),
  libraries : project_target,
)
