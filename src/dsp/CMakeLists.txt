cmake_minimum_required(VERSION 2.6)
project(cr42ynth.so)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LINK_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/generators/")
include_directories ("${PROJECT_SOURCE_DIR}/voice/")
include_directories ("${PROJECT_SOURCE_DIR}/")

add_executable(cr42ynth.so 
	#lv2_dsp.cpp
	CR42Ynth.cpp
	Control.cpp
	generators/Envelope.cpp
	generators/LFO.cpp
	generators/WTOscillator.cpp
	voice/Voice.cpp
	voice/ENVVoice.cpp
	voice/OscVoice.cpp
	voice/LFOVoice.cpp
)

target_link_libraries(cr42ynth.so ${LIBSNDFILE_LIBRARIES})