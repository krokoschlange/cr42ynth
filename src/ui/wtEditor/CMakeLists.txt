#project(test)

find_package(PkgConfig)

pkg_check_modules(CAIRO cairo REQUIRED)
include_directories( ${CAIRO_INCLUDE_DIRS})
link_directories   ( ${CAIRO_LIBRARY_DIRS})

pkg_check_modules(X11 x11 REQUIRED)
include_directories( ${X11_INCLUDE_DIRS} )
link_directories   ( ${X11_LIBRARY_DIRS} )


include_directories ("${PROJECT_SOURCE_DIR}/../avtk/avtk/")
include_directories ("${PROJECT_SOURCE_DIR}/../exprtk/")
include_directories ("${PROJECT_SOURCE_DIR}/../rtosc/include/")
include_directories ("${PROJECT_SOURCE_DIR}/common/")
include_directories ("${PROJECT_SOURCE_DIR}/ui/wtEditor/tools/")

add_executable(test.so
	../../common/common.cpp
	../../common/WaveformPart.cpp
	../../common/WavetableEditData.cpp
	../../common/WPFunction.cpp
	../../common/WPHarmonics.cpp
	../../common/WPSamples.cpp
	../../common/FftRealPair.cpp
	
	tools/FreeTool.cpp
	tools/SinHalfTool.cpp
	tools/SinSlopeTool.cpp
	tools/TriTool.cpp
	tools/WTTool.cpp
	
	CRSurfaceButton.cpp
	CustomScroll.cpp
	DragBar.cpp
	
	HarmonicsEditor.cpp
	HarmonicsView.cpp
	test.cpp
	TestUI.cpp
	ToolPanel.cpp
	WaveformView.cpp
	WTEditor.cpp
	WTView.cpp
)

target_link_libraries(test.so ${PROJECT_BINARY_DIR}/../rtosc/librtosc.a)
target_link_libraries(test.so ${PROJECT_BINARY_DIR}/../avtk/libavtk.a)
target_link_libraries(test.so ${CAIRO_LIBRARIES})
target_link_libraries(test.so ${X11_LIBRARIES})
