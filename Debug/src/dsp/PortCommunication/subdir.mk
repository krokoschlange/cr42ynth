################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/dsp/PortCommunication/DoubleMessageReceiver.cpp \
../src/dsp/PortCommunication/MessageReceiver.cpp \
../src/dsp/PortCommunication/PortCommunicator.cpp 

OBJS += \
./src/dsp/PortCommunication/DoubleMessageReceiver.o \
./src/dsp/PortCommunication/MessageReceiver.o \
./src/dsp/PortCommunication/PortCommunicator.o 

CPP_DEPS += \
./src/dsp/PortCommunication/DoubleMessageReceiver.d \
./src/dsp/PortCommunication/MessageReceiver.d \
./src/dsp/PortCommunication/PortCommunicator.d 


# Each subdirectory must supply rules for building sources it contributes
src/dsp/PortCommunication/%.o: ../src/dsp/PortCommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


