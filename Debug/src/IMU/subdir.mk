################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IMU/IMUnit.cpp \
../src/IMU/Matrix.cpp \
../src/IMU/Vector.cpp 

OBJS += \
./src/IMU/IMUnit.o \
./src/IMU/Matrix.o \
./src/IMU/Vector.o 

CPP_DEPS += \
./src/IMU/IMUnit.d \
./src/IMU/Matrix.d \
./src/IMU/Vector.d 


# Each subdirectory must supply rules for building sources it contributes
src/IMU/%.o: ../src/IMU/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++11 -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


