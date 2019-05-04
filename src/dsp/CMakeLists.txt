cmake_minimum_required(VERSION 2.6)
project(cr42ynth.so)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LINK_DIRS})

add_executable(cr42ynth.so 
	lv2_dsp.cpp
	WTOscillator.cpp
	LFO.cpp
	Control.cpp
)

target_link_libraries(cr42ynth.so ${LIBSNDFILE_LIBRARIES})