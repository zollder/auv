################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sys/I2C.cpp \
../src/sys/Timer.cpp 

OBJS += \
./src/sys/I2C.o \
./src/sys/Timer.o 

CPP_DEPS += \
./src/sys/I2C.d \
./src/sys/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/sys/%.o: ../src/sys/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


