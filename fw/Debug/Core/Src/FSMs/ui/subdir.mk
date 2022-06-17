################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/FSMs/ui/ui_draw.c \
../Core/Src/FSMs/ui/ui_fsm.c 

OBJS += \
./Core/Src/FSMs/ui/ui_draw.o \
./Core/Src/FSMs/ui/ui_fsm.o 

C_DEPS += \
./Core/Src/FSMs/ui/ui_draw.d \
./Core/Src/FSMs/ui/ui_fsm.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/FSMs/ui/%.o Core/Src/FSMs/ui/%.su: ../Core/Src/FSMs/ui/%.c Core/Src/FSMs/ui/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Core/Fonts -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/lcd -I../Core/Inc/API -I../Core/Inc/API/printf_dbg -I../Core/Inc/API/event_queue -I../Core/Inc/API/time_event -I../Core/Inc/API/ring_buffer -I../Core/Inc/API/buttons -I../Core/Inc/FSMs/event_manager -I../Core/Inc/FSMs/ui -I../Core/Inc/FSMs/drawer -I../Core/Inc/FSMs/feeder -I../Core/Inc/FSMs/power -I../Core/Inc/FSMs/temperature -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-FSMs-2f-ui

clean-Core-2f-Src-2f-FSMs-2f-ui:
	-$(RM) ./Core/Src/FSMs/ui/ui_draw.d ./Core/Src/FSMs/ui/ui_draw.o ./Core/Src/FSMs/ui/ui_draw.su ./Core/Src/FSMs/ui/ui_fsm.d ./Core/Src/FSMs/ui/ui_fsm.o ./Core/Src/FSMs/ui/ui_fsm.su

.PHONY: clean-Core-2f-Src-2f-FSMs-2f-ui

