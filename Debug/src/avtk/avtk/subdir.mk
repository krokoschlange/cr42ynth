################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../src/avtk/avtk/box.cxx \
../src/avtk/avtk/button.cxx \
../src/avtk/avtk/common.cxx \
../src/avtk/avtk/dial.cxx \
../src/avtk/avtk/dialog.cxx \
../src/avtk/avtk/envelope.cxx \
../src/avtk/avtk/eventeditor.cxx \
../src/avtk/avtk/filebrowser.cxx \
../src/avtk/avtk/group.cxx \
../src/avtk/avtk/helpers.cxx \
../src/avtk/avtk/image.cxx \
../src/avtk/avtk/list.cxx \
../src/avtk/avtk/listitem.cxx \
../src/avtk/avtk/midi.cxx \
../src/avtk/avtk/number.cxx \
../src/avtk/avtk/rotary.cxx \
../src/avtk/avtk/scroll.cxx \
../src/avtk/avtk/slider.cxx \
../src/avtk/avtk/spectrum.cxx \
../src/avtk/avtk/tester.cxx \
../src/avtk/avtk/text.cxx \
../src/avtk/avtk/theme.cxx \
../src/avtk/avtk/ui.cxx \
../src/avtk/avtk/utils.cxx \
../src/avtk/avtk/waveform.cxx \
../src/avtk/avtk/widget.cxx 

CXX_DEPS += \
./src/avtk/avtk/box.d \
./src/avtk/avtk/button.d \
./src/avtk/avtk/common.d \
./src/avtk/avtk/dial.d \
./src/avtk/avtk/dialog.d \
./src/avtk/avtk/envelope.d \
./src/avtk/avtk/eventeditor.d \
./src/avtk/avtk/filebrowser.d \
./src/avtk/avtk/group.d \
./src/avtk/avtk/helpers.d \
./src/avtk/avtk/image.d \
./src/avtk/avtk/list.d \
./src/avtk/avtk/listitem.d \
./src/avtk/avtk/midi.d \
./src/avtk/avtk/number.d \
./src/avtk/avtk/rotary.d \
./src/avtk/avtk/scroll.d \
./src/avtk/avtk/slider.d \
./src/avtk/avtk/spectrum.d \
./src/avtk/avtk/tester.d \
./src/avtk/avtk/text.d \
./src/avtk/avtk/theme.d \
./src/avtk/avtk/ui.d \
./src/avtk/avtk/utils.d \
./src/avtk/avtk/waveform.d \
./src/avtk/avtk/widget.d 

OBJS += \
./src/avtk/avtk/box.o \
./src/avtk/avtk/button.o \
./src/avtk/avtk/common.o \
./src/avtk/avtk/dial.o \
./src/avtk/avtk/dialog.o \
./src/avtk/avtk/envelope.o \
./src/avtk/avtk/eventeditor.o \
./src/avtk/avtk/filebrowser.o \
./src/avtk/avtk/group.o \
./src/avtk/avtk/helpers.o \
./src/avtk/avtk/image.o \
./src/avtk/avtk/list.o \
./src/avtk/avtk/listitem.o \
./src/avtk/avtk/midi.o \
./src/avtk/avtk/number.o \
./src/avtk/avtk/rotary.o \
./src/avtk/avtk/scroll.o \
./src/avtk/avtk/slider.o \
./src/avtk/avtk/spectrum.o \
./src/avtk/avtk/tester.o \
./src/avtk/avtk/text.o \
./src/avtk/avtk/theme.o \
./src/avtk/avtk/ui.o \
./src/avtk/avtk/utils.o \
./src/avtk/avtk/waveform.o \
./src/avtk/avtk/widget.o 


# Each subdirectory must supply rules for building sources it contributes
src/avtk/avtk/%.o: ../src/avtk/avtk/%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


