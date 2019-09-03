cmake_minimum_required(VERSION 2.6)

add_subdirectory(wtEditor/)

#project(cr42ynth_ui.so)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LINK_DIRS})

pkg_check_modules(CAIRO cairo REQUIRED)
include_directories( ${CAIRO_INCLUDE_DIRS})
link_directories   ( ${CAIRO_LIBRARY_DIRS})

pkg_check_modules(X11 x11 REQUIRED)
include_directories( ${X11_INCLUDE_DIRS} )
link_directories   ( ${X11_LIBRARY_DIRS} )

include_directories ("${PROJECT_SOURCE_DIR}/../rtosc/include/")
include_directories ("${PROJECT_SOURCE_DIR}/../avtk/avtk/")
include_directories ("${PROJECT_SOURCE_DIR}/../exprtk/")
include_directories ("${PROJECT_SOURCE_DIR}/dsp/Generators/")
include_directories ("${PROJECT_SOURCE_DIR}/common/")
include_directories ("${PROJECT_SOURCE_DIR}/ui/lv2/")
include_directories ("${PROJECT_SOURCE_DIR}/ui/")
include_directories ("${PROJECT_SOURCE_DIR}/ui/wtEditor")
include_directories ("${PROJECT_SOURCE_DIR}/ui/wtEditor/tools")

add_executable(cr42ynth_ui.so
	../common/OSCEvent.cpp
	../common/CR42YnthCommunicator.cpp
	lv2/CR42YnthUILV2.cpp
	lv2/lv2UI.cpp
	
	../common/WaveformPart.cpp
	../common/WavetableEditData.cpp
	../common/WPFunction.cpp
	../common/WPHarmonics.cpp
	../common/WPSamples.cpp
	../common/FftRealPair.cpp
	
	wtEditor/tools/FreeTool.cpp
	wtEditor/tools/SinHalfTool.cpp
	wtEditor/tools/SinSlopeTool.cpp
	wtEditor/tools/TriTool.cpp
	wtEditor/tools/WTTool.cpp
	
	wtEditor/CRSurfaceButton.cpp
	wtEditor/CustomScroll.cpp
	wtEditor/DragBar.cpp
	
	wtEditor/HarmonicsEditor.cpp
	wtEditor/HarmonicsView.cpp
	wtEditor/ToolPanel.cpp
	wtEditor/WaveformView.cpp
	wtEditor/WTEditor.cpp
	wtEditor/WTView.cpp
	
	CR42YnthUI.cpp
)

target_link_libraries(cr42ynth_ui.so ${PROJECT_BINARY_DIR}/../rtosc/librtosc.a)
target_link_libraries(cr42ynth_ui.so ${PROJECT_BINARY_DIR}/../avtk/libavtk.a)
target_link_libraries(cr42ynth_ui.so ${LIBSNDFILE_LIBRARIES})
target_link_libraries(cr42ynth_ui.so ${CAIRO_LIBRARIES})
target_link_libraries(cr42ynth_ui.so ${X11_LIBRARIES})
