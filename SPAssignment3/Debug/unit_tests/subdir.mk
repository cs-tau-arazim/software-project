################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../unit_tests/sp_bpqueue_unit_test.c \
../unit_tests/sp_logger_unit_test.c 

OBJS += \
./unit_tests/sp_bpqueue_unit_test.o \
./unit_tests/sp_logger_unit_test.o 

C_DEPS += \
./unit_tests/sp_bpqueue_unit_test.d \
./unit_tests/sp_logger_unit_test.d 


# Each subdirectory must supply rules for building sources it contributes
unit_tests/%.o: ../unit_tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


