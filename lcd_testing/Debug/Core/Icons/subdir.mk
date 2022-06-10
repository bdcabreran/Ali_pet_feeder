################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Icons/battery_icon.c \
../Core/Icons/recording_icon.c 

OBJS += \
./Core/Icons/battery_icon.o \
./Core/Icons/recording_icon.o 

C_DEPS += \
./Core/Icons/battery_icon.d \
./Core/Icons/recording_icon.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Icons/%.o Core/Icons/%.su: ../Core/Icons/%.c Core/Icons/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Fonts -I../Core/lcd -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Icons

clean-Core-2f-Icons:
	-$(RM) ./Core/Icons/battery_icon.d ./Core/Icons/battery_icon.o ./Core/Icons/battery_icon.su ./Core/Icons/recording_icon.d ./Core/Icons/recording_icon.o ./Core/Icons/recording_icon.su

.PHONY: clean-Core-2f-Icons

