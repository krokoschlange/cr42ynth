cmake_minimum_required(VERSION 2.6)
project(cr42ynth.so)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LINK_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/modifiers/")
include_directories ("${PROJECT_SOURCE_DIR}/generators/")
include_directories ("${PROJECT_SOURCE_DIR}/playheads/")
include_directories ("${PROJECT_SOURCE_DIR}/")

add_executable(cr42ynth.so 
	lv2_dsp.cpp
	CR42Ynth.cpp
	CR42YnthURIS.cpp
	Control.cpp
	Controller.cpp
	modifiers/Envelope.cpp
	modifiers/LFO.cpp
	generators/WTOscillator.cpp
	Voice.cpp
	playheads/ENVPlayhead.cpp
	playheads/OscPlayhead.cpp
	playheads/LFOPlayhead.cpp
)

target_link_libraries(cr42ynth.so ${LIBSNDFILE_LIBRARIES})