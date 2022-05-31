################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/API/ring_buffer.c 

OBJS += \
./Core/Src/API/ring_buffer.o 

C_DEPS += \
./Core/Src/API/ring_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/API/%.o Core/Src/API/%.su: ../Core/Src/API/%.c Core/Src/API/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/API -I../Core/Inc/printf_dbg -I../Core/Inc/time_event -I../Core/Inc/event_queue -I../Core/Inc/FSMs/event_manager -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-API

clean-Core-2f-Src-2f-API:
	-$(RM) ./Core/Src/API/ring_buffer.d ./Core/Src/API/ring_buffer.o ./Core/Src/API/ring_buffer.su

.PHONY: clean-Core-2f-Src-2f-API

