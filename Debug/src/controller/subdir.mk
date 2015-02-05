################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/controller/AltitudeController.cc \
../src/controller/HeadingController.cc \
../src/controller/HorizontalMotion.cc 

CC_DEPS += \
./src/controller/AltitudeController.d \
./src/controller/HeadingController.d \
./src/controller/HorizontalMotion.d 

OBJS += \
./src/controller/AltitudeController.o \
./src/controller/HeadingController.o \
./src/controller/HorizontalMotion.o 


# Each subdirectory must supply rules for building sources it contributes
src/controller/%.o: ../src/controller/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++11 -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


