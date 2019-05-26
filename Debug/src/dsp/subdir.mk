################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/dsp/DoubleControl.cpp 

OBJS += \
./src/dsp/DoubleControl.o 

CPP_DEPS += \
./src/dsp/DoubleControl.d 


# Each subdirectory must supply rules for building sources it contributes
src/dsp/%.o: ../src/dsp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


