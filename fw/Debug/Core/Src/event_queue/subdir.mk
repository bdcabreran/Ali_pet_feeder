################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/event_queue/event_queue.c 

OBJS += \
./Core/Src/event_queue/event_queue.o 

C_DEPS += \
./Core/Src/event_queue/event_queue.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/event_queue/%.o Core/Src/event_queue/%.su: ../Core/Src/event_queue/%.c Core/Src/event_queue/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/API -I../Core/Inc/printf_dbg -I../Core/Inc/time_event -I../Core/Inc/event_queue -I../Core/Inc/FSMs/event_manager -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-event_queue

clean-Core-2f-Src-2f-event_queue:
	-$(RM) ./Core/Src/event_queue/event_queue.d ./Core/Src/event_queue/event_queue.o ./Core/Src/event_queue/event_queue.su

.PHONY: clean-Core-2f-Src-2f-event_queue

