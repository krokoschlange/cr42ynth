#include_directories ("${PROJECT_SOURCE_DIR}/src/avtk/")
#
#
#find_package(PkgConfig)
#
#pkg_check_modules(CAIRO cairo REQUIRED)
#include_directories( ${CAIRO_INCLUDE_DIRS})
#link_directories   ( ${CAIRO_LIBRARY_DIRS})
#
#pkg_check_modules(X11 x11 REQUIRED)
#include_directories( ${X11_INCLUDE_DIRS} )
#link_directories   ( ${X11_LIBRARY_DIRS} )

project(CR42Ynth)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

add_subdirectory(dsp/)
add_subdirectory(ui/)
