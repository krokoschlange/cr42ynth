################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/dsp/Controllers/Controller.cpp \
../src/dsp/Controllers/LFOController.cpp 

OBJS += \
./src/dsp/Controllers/Controller.o \
./src/dsp/Controllers/LFOController.o 

CPP_DEPS += \
./src/dsp/Controllers/Controller.d \
./src/dsp/Controllers/LFOController.d 


# Each subdirectory must supply rules for building sources it contributes
src/dsp/Controllers/%.o: ../src/dsp/Controllers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


