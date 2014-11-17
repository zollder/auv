################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/imu/IMUnit.cpp \
../src/imu/Matrix.cpp \
../src/imu/Vector.cpp 

OBJS += \
./src/imu/IMUnit.o \
./src/imu/Matrix.o \
./src/imu/Vector.o 

CPP_DEPS += \
./src/imu/IMUnit.d \
./src/imu/Matrix.d \
./src/imu/Vector.d 


# Each subdirectory must supply rules for building sources it contributes
src/imu/%.o: ../src/imu/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++11 -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


