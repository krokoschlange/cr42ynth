################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/avtk/avtk/pugl/pugl_x11.c 

OBJS += \
./src/avtk/avtk/pugl/pugl_x11.o 

C_DEPS += \
./src/avtk/avtk/pugl/pugl_x11.d 


# Each subdirectory must supply rules for building sources it contributes
src/avtk/avtk/pugl/%.o: ../src/avtk/avtk/pugl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


