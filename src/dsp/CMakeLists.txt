cmake_minimum_required(VERSION 3.10)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)

include_directories(${LIBSNDFILE_INCLUDE_DIRS})

include_directories ("${PROJECT_SOURCE_DIR}/src/dsp/")
include_directories ("${PROJECT_SOURCE_DIR}/src/dsp/Generators/")
include_directories ("${PROJECT_SOURCE_DIR}/src/dsp/lv2/")

include_directories ("${PROJECT_SOURCE_DIR}/src/common/")
include_directories ("${PROJECT_SOURCE_DIR}/src/common/lv2/")
include_directories ("${PROJECT_SOURCE_DIR}/rtosc/include/")
include_directories ("${PROJECT_SOURCE_DIR}/exprtk/")

add_library(cr42ynth SHARED
	../common/FftRealPair.cpp
	../common/CR42YnthCommunicator.cpp
	../common/common.cpp
	../common/Control.cpp
	../common/ControlListener.cpp
	../common/ModulationControls.cpp
	../common/OSCEvent.cpp
	../common/OSCEventListener.cpp
	../common/OscillatorControls.cpp
	../common/WavetableEditData.cpp
	../common/WaveformPart.cpp
	../common/WPFunction.cpp
	../common/WPHarmonics.cpp
	../common/WPSamples.cpp
	../common/AutomationData.cpp
	../common/lv2/CR42YnthLV2URIS.cpp
	
	WTOscillator.cpp
	
	
	lv2/lv2DSP.cpp
	lv2/CR42YnthLV2.cpp
	
	CR42YnthDSP.cpp
	Voice.cpp
	
	Automation.cpp
	AutomationHandler.cpp
)

set_target_properties(cr42ynth PROPERTIES PREFIX "") 

link_directories(${LIBSNDFILE_LINK_DIRS})

target_link_libraries(cr42ynth ${PROJECT_BINARY_DIR}/rtosc/librtosc.a)
target_link_libraries(cr42ynth ${LIBSNDFILE_LIBRARIES})
target_compile_options(cr42ynth PRIVATE -Werror -Wall -Wextra)
