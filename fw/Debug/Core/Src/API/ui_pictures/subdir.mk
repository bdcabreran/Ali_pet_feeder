################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/API/ui_pictures/icons.c \
../Core/Src/API/ui_pictures/static_background.c 

OBJS += \
./Core/Src/API/ui_pictures/icons.o \
./Core/Src/API/ui_pictures/static_background.o 

C_DEPS += \
./Core/Src/API/ui_pictures/icons.d \
./Core/Src/API/ui_pictures/static_background.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/API/ui_pictures/%.o Core/Src/API/ui_pictures/%.su: ../Core/Src/API/ui_pictures/%.c Core/Src/API/ui_pictures/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Core/Inc/FSMs/event_manager -I../Core/Fonts -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/lcd -I../Core/Inc/FSMs/event_manager -I../Core/Inc/API -I../Core/Inc/API/printf_dbg -I../Core/Inc/API/event_queue -I../Core/Inc/API/time_event -I../Core/Inc/API/ring_buffer -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-API-2f-ui_pictures

clean-Core-2f-Src-2f-API-2f-ui_pictures:
	-$(RM) ./Core/Src/API/ui_pictures/icons.d ./Core/Src/API/ui_pictures/icons.o ./Core/Src/API/ui_pictures/icons.su ./Core/Src/API/ui_pictures/static_background.d ./Core/Src/API/ui_pictures/static_background.o ./Core/Src/API/ui_pictures/static_background.su

.PHONY: clean-Core-2f-Src-2f-API-2f-ui_pictures

