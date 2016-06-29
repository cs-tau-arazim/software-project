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
../SPBPriorityQueue.c \
../SPConfig.c \
../SPList.c \
../SPListElement.c \
../SPLogger.c \
../SPPoint.c 

OBJS += \
./KDArray.o \
./KDTreeNode.o \
./SPBPriorityQueue.o \
./SPConfig.o \
./SPImageProc.o \
./SPList.o \
./SPListElement.o \
./SPLogger.o \
./SPPoint.o \
./main.o 

CPP_DEPS += \
./SPImageProc.d \
./main.d 

C_DEPS += \
./KDArray.d \
./KDTreeNode.d \
./SPBPriorityQueue.d \
./SPConfig.d \
./SPList.d \
./SPListElement.d \
./SPLogger.d \
./SPPoint.d 


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


