################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SPImageProc.cpp \
../main.cpp 

C_SRCS += \
../KDArray.c \
../KDTreeNode.c \
../SPConfig.c \
../SPLogger.c \
../SPPoint.c \
../main_aux.c 

OBJS += \
./KDArray.o \
./KDTreeNode.o \
./SPConfig.o \
./SPImageProc.o \
./SPLogger.o \
./SPPoint.o \
./main.o \
./main_aux.o 

CPP_DEPS += \
./SPImageProc.d \
./main.d 

C_DEPS += \
./KDArray.d \
./KDTreeNode.d \
./SPConfig.d \
./SPLogger.d \
./SPPoint.d \
./main_aux.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/lib/opencv-3.1.0/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


