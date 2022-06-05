################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/API/time_event/time_event.c \
../Core/Src/API/time_event/time_irq_update.c 

OBJS += \
./Core/Src/API/time_event/time_event.o \
./Core/Src/API/time_event/time_irq_update.o 

C_DEPS += \
./Core/Src/API/time_event/time_event.d \
./Core/Src/API/time_event/time_irq_update.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/API/time_event/%.o Core/Src/API/time_event/%.su: ../Core/Src/API/time_event/%.c Core/Src/API/time_event/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/FSMs/event_manager -I../Core/Inc/API -I../Core/Inc/API/printf_dbg -I../Core/Inc/API/event_queue -I../Core/Inc/API/time_event -I../Core/Inc/API/ring_buffer -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-API-2f-time_event

clean-Core-2f-Src-2f-API-2f-time_event:
	-$(RM) ./Core/Src/API/time_event/time_event.d ./Core/Src/API/time_event/time_event.o ./Core/Src/API/time_event/time_event.su ./Core/Src/API/time_event/time_irq_update.d ./Core/Src/API/time_event/time_irq_update.o ./Core/Src/API/time_event/time_irq_update.su

.PHONY: clean-Core-2f-Src-2f-API-2f-time_event

