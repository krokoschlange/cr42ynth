cmake_minimum_required(VERSION 2.6)
project(cr42ynth.so)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)

add_executable(cr42ynth.so
	../common/CR42YnthCommunicator.cpp
	../common/Control.cpp
	../common/OSCEvent.cpp
	../common/ControlListener.cpp
	
	Generators/OscillatorVoiceData.cpp
	Generators/Generator.cpp
	Generators/WTOscillator.cpp
	Generators/LFO.cpp
	
	
	lv2/lv2DSP.cpp
	lv2/CR42YnthLV2.cpp
	
	CR42YnthDSP.cpp
	Voice.cpp
	Property.cpp
)

include_directories(${LIBSNDFILE_INCLUDE_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/../../rtosc/include/rtosc/")
include_directories ("${PROJECT_SOURCE_DIR}/Generators/")
include_directories ("${PROJECT_SOURCE_DIR}/../common/")
include_directories ("${PROJECT_SOURCE_DIR}/lv2/")
include_directories ("${PROJECT_SOURCE_DIR}/")


link_directories(${LIBSNDFILE_LINK_DIRS})

target_link_libraries(cr42ynth.so ${PROJECT_BINARY_DIR}/../../rtosc/librtosc.a)
target_link_libraries(cr42ynth.so ${LIBSNDFILE_LIBRARIES})