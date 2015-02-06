################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/imu/ImuThread.cc 

CPP_SRCS += \
../src/imu/IMU.cpp 

CC_DEPS += \
./src/imu/ImuThread.d 

OBJS += \
./src/imu/IMU.o \
./src/imu/ImuThread.o 

CPP_DEPS += \
./src/imu/IMU.d 


# Each subdirectory must supply rules for building sources it contributes
src/imu/%.o: ../src/imu/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++11 -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O3 -g3 -c -fmessage-length=0 -w -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/imu/%.o: ../src/imu/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++11 -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O3 -g3 -c -fmessage-length=0 -w -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


