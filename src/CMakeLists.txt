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

#add_subdirectory(ui/)
add_subdirectory(dsp/)
