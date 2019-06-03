cmake_minimum_required(VERSION 2.6)
project(cr42ynth.so)

find_package(PkgConfig)

pkg_check_modules(LIBSNDFILE sndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LINK_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/Controllers/")
include_directories ("${PROJECT_SOURCE_DIR}/Generators/")
include_directories ("${PROJECT_SOURCE_DIR}/../common/")
include_directories ("${PROJECT_SOURCE_DIR}/")

add_executable(cr42ynth.so
	
	../common/DoubleControl.cpp
	../common/BoolControl.cpp
	../common/IntControl.cpp
	../common/DefinitionHandler.cpp
	../common/PortCommunication/PortCommunicator.cpp
	../common/PortCommunication/MessageReceiver.cpp
	
	Controllers/LFOController.cpp
	Controllers/EnvelopeController.cpp
	Controllers/OscillatorController.cpp
	Controllers/Controller.cpp
	
	
	Generators/LFO.cpp
	Generators/Envelope.cpp
	Generators/WTOscillator.cpp
	
	lv2_dsp.cpp
	ModulatableControl.cpp
	ModulatableDoubleControl.cpp
	ModulatableIntControl.cpp
	DSPPortCommunicator.cpp
	Voice.cpp
	CR42Ynth.cpp
)

target_link_libraries(cr42ynth.so ${LIBSNDFILE_LIBRARIES})