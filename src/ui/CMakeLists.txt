cmake_minimum_required(VERSION 2.6)

#add_subdirectory(wtEditor/)

#project(cr42ynth_ui.so)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LINK_DIRS})

pkg_check_modules(GTKMM gtkmm-2.4 REQUIRED)
include_directories( ${GTKMM_INCLUDE_DIRS})
link_directories   ( ${GTKMM_LIBRARY_DIRS})

include_directories ("${PROJECT_SOURCE_DIR}/../rtosc/include/")
include_directories ("${PROJECT_SOURCE_DIR}/../exprtk/")
include_directories ("${PROJECT_SOURCE_DIR}/dsp/Generators/")
include_directories ("${PROJECT_SOURCE_DIR}/common/")
include_directories ("${PROJECT_SOURCE_DIR}/common/lv2/")
include_directories ("${PROJECT_SOURCE_DIR}/ui/lv2/")
include_directories ("${PROJECT_SOURCE_DIR}/ui/")
include_directories ("${PROJECT_SOURCE_DIR}/ui/tools/")

add_executable(cr42ynth_ui.so
	../common/OSCEvent.cpp
	../common/OSCEventListener.cpp
	../common/OscillatorControls.cpp
	../common/Control.cpp
	../common/ControlListener.cpp
	../common/common.cpp
	../common/CR42YnthCommunicator.cpp
	../common/CR42YConfig.cpp
	
	
	../common/lv2/CR42YnthLV2Communicator.cpp
	../common/lv2/CommunicatorMessage.cpp
	
	lv2/CR42YnthUILV2.cpp
	lv2/lv2UI.cpp
	
	../common/WaveformPart.cpp
	../common/WavetableEditData.cpp
	../common/WPFunction.cpp
	../common/WPHarmonics.cpp
	../common/WPSamples.cpp
	../common/FftRealPair.cpp
	
	WavetableEditController.cpp
	
	tools/FreeTool.cpp
	tools/SinHalfTool.cpp
	tools/SinSlopeTool.cpp
	tools/TriTool.cpp
	tools/WTTool.cpp
	
	ControlConnector.cpp
	
	#wtEditor/CRSurfaceButton.cpp
	#wtEditor/CustomScroll.cpp
	#wtEditor/DragBar.cpp
	
	#wtEditor/HarmonicsEditor.cpp
	#wtEditor/HarmonicsView.cpp
	#wtEditor/ToolPanel.cpp
	#wtEditor/WaveformView.cpp
	#wtEditor/WTEditor.cpp
	#wtEditor/WTView.cpp
	
	#PanelSwitcher.cpp
	#PanelSwitcherEntry.cpp
	#LineEntry.cpp
	CR42YnthUI.cpp
	CR42YWidget.cpp
	CR42YTheme.cpp
	CR42YButton.cpp
	CR42YLabel.cpp
	CR42YBoxVScale.cpp
	CR42YRelativeContainer.cpp
	CR42YUI.cpp
	CR42YWaveformEditor.cpp
	CR42YWavetableEditor.cpp
	CR42YHarmonicsView.cpp
	CR42YWavetableView.cpp
	CR42YWavetableViewItem.cpp
	CR42YEntry.cpp
	CR42YIntegerEntry.cpp
	CR42YIntegerEditor.cpp
	CR42YVScrollbar.cpp
	CR42YWaveformEditorControlPanel.cpp
	CR42YHScrollbar.cpp
	CR42YHarmonicsEditor.cpp
	CR42YToolSelector.cpp
	CR42YToggle.cpp
	CR42YToggleSelector.cpp
	CR42YWFPartEditor.cpp
	CR42YWFBulkEditor.cpp
	CR42YDial.cpp
	CR42YOSCSettings.cpp
	CR42YOSCPanel.cpp
	CR42YOSCPanelScroll.cpp
	CR42YGrid.cpp
	CR42YControlDial.cpp
	CR42YWFView.cpp
	OSCSettingsController.cpp
	CR42YControlToggle.cpp
	
	helpers.cpp
)

target_link_libraries(cr42ynth_ui.so ${PROJECT_BINARY_DIR}/../rtosc/librtosc.a)
target_link_libraries(cr42ynth_ui.so ${LIBSNDFILE_LIBRARIES})
target_link_libraries(cr42ynth_ui.so ${GTKMM_LIBRARIES})
