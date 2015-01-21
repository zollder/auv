################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sys/I2C.cpp \
../src/sys/PWM.cpp \
../src/sys/Timer.cpp 

CC_SRCS += \
../src/sys/BaseThread.cc \
../src/sys/FdTimer.cc 

OBJS += \
./src/sys/BaseThread.o \
./src/sys/FdTimer.o \
./src/sys/I2C.o \
./src/sys/PWM.o \
./src/sys/Timer.o 

CC_DEPS += \
./src/sys/BaseThread.d \
./src/sys/FdTimer.d 

CPP_DEPS += \
./src/sys/I2C.d \
./src/sys/PWM.d \
./src/sys/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/sys/%.o: ../src/sys/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/%.o: ../src/sys/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


