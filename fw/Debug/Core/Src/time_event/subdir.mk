################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/time_event/time_event.c \
../Core/Src/time_event/time_irq_update.c 

OBJS += \
./Core/Src/time_event/time_event.o \
./Core/Src/time_event/time_irq_update.o 

C_DEPS += \
./Core/Src/time_event/time_event.d \
./Core/Src/time_event/time_irq_update.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/time_event/%.o Core/Src/time_event/%.su: ../Core/Src/time_event/%.c Core/Src/time_event/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/API -I../Core/Inc/printf_dbg -I../Core/Inc/time_event -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-time_event

clean-Core-2f-Src-2f-time_event:
	-$(RM) ./Core/Src/time_event/time_event.d ./Core/Src/time_event/time_event.o ./Core/Src/time_event/time_event.su ./Core/Src/time_event/time_irq_update.d ./Core/Src/time_event/time_irq_update.o ./Core/Src/time_event/time_irq_update.su

.PHONY: clean-Core-2f-Src-2f-time_event

