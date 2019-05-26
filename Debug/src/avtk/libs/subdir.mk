################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../src/avtk/libs/pffft.cxx 

CXX_DEPS += \
./src/avtk/libs/pffft.d 

OBJS += \
./src/avtk/libs/pffft.o 


# Each subdirectory must supply rules for building sources it contributes
src/avtk/libs/%.o: ../src/avtk/libs/%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


