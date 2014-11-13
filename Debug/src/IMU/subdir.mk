################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IMU/L3G.cpp \
../src/IMU/LSM303.cpp 

OBJS += \
./src/IMU/L3G.o \
./src/IMU/LSM303.o 

CPP_DEPS += \
./src/IMU/L3G.d \
./src/IMU/LSM303.d 


# Each subdirectory must supply rules for building sources it contributes
src/IMU/%.o: ../src/IMU/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/usr/arm-linux-gnueabihf/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


