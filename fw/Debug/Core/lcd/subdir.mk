################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/lcd/ili9488.c \
../Core/lcd/lcd_api.c \
../Core/lcd/lcd_spi_io.c 

OBJS += \
./Core/lcd/ili9488.o \
./Core/lcd/lcd_api.o \
./Core/lcd/lcd_spi_io.o 

C_DEPS += \
./Core/lcd/ili9488.d \
./Core/lcd/lcd_api.d \
./Core/lcd/lcd_spi_io.d 


# Each subdirectory must supply rules for building sources it contributes
Core/lcd/%.o Core/lcd/%.su: ../Core/lcd/%.c Core/lcd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Core/Inc/FSMs/event_manager -I../Core/Fonts -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/lcd -I../Core/Inc/API -I../Core/Inc/API/printf_dbg -I../Core/Inc/API/event_queue -I../Core/Inc/API/time_event -I../Core/Inc/API/ring_buffer -I../Core/Inc/FSMs/ui -I../Core/Inc/API/buttons -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-lcd

clean-Core-2f-lcd:
	-$(RM) ./Core/lcd/ili9488.d ./Core/lcd/ili9488.o ./Core/lcd/ili9488.su ./Core/lcd/lcd_api.d ./Core/lcd/lcd_api.o ./Core/lcd/lcd_api.su ./Core/lcd/lcd_spi_io.d ./Core/lcd/lcd_spi_io.o ./Core/lcd/lcd_spi_io.su

.PHONY: clean-Core-2f-lcd

