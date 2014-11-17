################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/devices/L3G.cpp \
../src/devices/LSM303.cpp 

OBJS += \
./src/devices/L3G.o \
./src/devices/LSM303.o 

CPP_DEPS += \
./src/devices/L3G.d \
./src/devices/LSM303.d 


# Each subdirectory must supply rules for building sources it contributes
src/devices/%.o: ../src/devices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++11 -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


