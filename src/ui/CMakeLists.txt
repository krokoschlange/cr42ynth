cmake_minimum_required(VERSION 3.10)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LINK_DIRS})

pkg_check_modules(GTKMM gtkmm-2.4 REQUIRED)
include_directories(SYSTEM ${GTKMM_INCLUDE_DIRS})
link_directories   ( ${GTKMM_LIBRARY_DIRS})

include_directories ("${PROJECT_SOURCE_DIR}/rtosc/include/")
include_directories ("${PROJECT_SOURCE_DIR}/exprtk/")
include_directories ("${PROJECT_SOURCE_DIR}/src/dsp/Generators/")
include_directories ("${PROJECT_SOURCE_DIR}/src/common/")
include_directories ("${PROJECT_SOURCE_DIR}/src/common/lv2/")
include_directories ("${PROJECT_SOURCE_DIR}/src/ui/lv2/")
include_directories ("${PROJECT_SOURCE_DIR}/src/ui/")
include_directories ("${PROJECT_SOURCE_DIR}/src/ui/tools/")
include_directories ("${PROJECT_SOURCE_DIR}/src/dsp/lv2/")

add_library(cr42ynth_ui SHARED
	../common/OSCEvent.cpp
	../common/OSCEventListener.cpp
	../common/OscillatorControls.cpp
	../common/Control.cpp
	../common/ControlListener.cpp
	../common/common.cpp
	../common/CR42YnthCommunicator.cpp
	../common/CR42YConfig.cpp
	../common/ModulationControls.cpp
	../common/AutomationData.cpp
	
	
	../common/lv2/CR42YnthLV2URIS.cpp
	
	lv2/CR42YnthUILV2.cpp
	lv2/lv2UI.cpp
	
	../common/WaveformPart.cpp
	../common/WavetableEditData.cpp
	../common/WPFunction.cpp
	../common/WPHarmonics.cpp
	../common/WPSamples.cpp
	../common/FftRealPair.cpp
	
	WavetableEditController.cpp
	AutomationEditController.cpp
	
	tools/FreeTool.cpp
	tools/SinHalfTool.cpp
	tools/SinSlopeTool.cpp
	tools/TriTool.cpp
	tools/WTTool.cpp
	
	ControlConnector.cpp
	
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
	CR42YModulationEditor.cpp
	CR42YSquareContainer.cpp
	CR42YAutomationSampleEditor.cpp
	CR42YAutomationEditor.cpp
	CR42YAutomationSelector.cpp
	CR42YAutomationItem.cpp
	CR42YAutomationSettings.cpp
	CR42YControlToggleSelector.cpp
	CR42YControlIntegerEditor.cpp
	CR42YAutomationDial.cpp
	CR42YDetuneEditor.cpp
	
	helpers.cpp
)

set_target_properties(cr42ynth_ui PROPERTIES PREFIX "")

target_link_libraries(cr42ynth_ui ${PROJECT_BINARY_DIR}/rtosc/librtosc.a)
target_link_libraries(cr42ynth_ui ${LIBSNDFILE_LIBRARIES})
target_link_libraries(cr42ynth_ui ${GTKMM_LIBRARIES})
target_compile_options(cr42ynth_ui PRIVATE -Werror -Wall -Wextra)

add_executable(cr42ynth_ui_dummy
	main.cpp
)

target_link_libraries(cr42ynth_ui_dummy cr42ynth_ui)
target_link_libraries(cr42ynth_ui_dummy ${PROJECT_BINARY_DIR}/rtosc/librtosc.a)
target_link_libraries(cr42ynth_ui_dummy ${LIBSNDFILE_LIBRARIES})
target_link_libraries(cr42ynth_ui_dummy ${GTKMM_LIBRARIES})
target_compile_options(cr42ynth_ui_dummy PRIVATE -Werror -Wall -Wextra)
