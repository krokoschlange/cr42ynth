cmake_minimum_required(VERSION 2.6)
project(cr42ynth_ui.so)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories ("${PROJECT_SOURCE_DIR}/../avtk/avtk/")
include_directories ("${PROJECT_SOURCE_DIR}/../avtk/avtk/pugl/")

find_package(PkgConfig)

pkg_check_modules(CAIRO cairo REQUIRED)
include_directories( ${CAIRO_INCLUDE_DIRS})
link_directories   ( ${CAIRO_LIBRARY_DIRS})

pkg_check_modules(X11 x11 REQUIRED)
include_directories( ${X11_INCLUDE_DIRS} )
link_directories   ( ${X11_LIBRARY_DIRS} )

add_executable(cr42ynth_ui.so
	lv2_ui.cpp
	CR42YnthUI.cpp
)

target_link_libraries(cr42ynth_ui.so ${PROJECT_SOURCE_DIR}/libavtk.a)
target_link_libraries(cr42ynth_ui.so ${CAIRO_LIBRARIES})
target_link_libraries(cr42ynth_ui.so ${X11_LIBRARIES})