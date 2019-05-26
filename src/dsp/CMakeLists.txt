cmake_minimum_required(VERSION 2.6)
project(cr42ynth.so)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LINK_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/Controllers/")
include_directories ("${PROJECT_SOURCE_DIR}/Generators/")
include_directories ("${PROJECT_SOURCE_DIR}/PortCommunication/")
include_directories ("${PROJECT_SOURCE_DIR}/../")
include_directories ("${PROJECT_SOURCE_DIR}/")

add_executable(cr42ynth.so 
	DoubleControl.cpp
	Controllers/Controller.cpp
	Controllers/LFOController.cpp
	Generators/LFO.cpp
	PortCommunication/DoubleMessageReceiver.cpp
	PortCommunication/MessageReceiver.cpp
	PortCommunication/PortCommunicator.cpp
	../DefinitionHandler.cpp
)

target_link_libraries(cr42ynth.so ${LIBSNDFILE_LIBRARIES})