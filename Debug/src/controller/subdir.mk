################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/controller/HeadingMotion.cc \
../src/controller/HorizontalMotion.cc \
../src/controller/VerticalMotion.cc 

CC_DEPS += \
./src/controller/HeadingMotion.d \
./src/controller/HorizontalMotion.d \
./src/controller/VerticalMotion.d 

OBJS += \
./src/controller/HeadingMotion.o \
./src/controller/HorizontalMotion.o \
./src/controller/VerticalMotion.o 


# Each subdirectory must supply rules for building sources it contributes
src/controller/%.o: ../src/controller/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabi-g++ -I/usr/arm-linux-gnueabi/include/c++/4.7.3/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


