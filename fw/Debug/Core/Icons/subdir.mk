################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Icons/back_icon.c \
../Core/Icons/batt_error_icon.c \
../Core/Icons/batt_warning_icon.c \
../Core/Icons/battery_icon.c \
../Core/Icons/drawers_icon.c \
../Core/Icons/enable_icon.c \
../Core/Icons/play_icon.c \
../Core/Icons/recording_icon.c \
../Core/Icons/thermostat_icon.c \
../Core/Icons/trash_icon.c 

OBJS += \
./Core/Icons/back_icon.o \
./Core/Icons/batt_error_icon.o \
./Core/Icons/batt_warning_icon.o \
./Core/Icons/battery_icon.o \
./Core/Icons/drawers_icon.o \
./Core/Icons/enable_icon.o \
./Core/Icons/play_icon.o \
./Core/Icons/recording_icon.o \
./Core/Icons/thermostat_icon.o \
./Core/Icons/trash_icon.o 

C_DEPS += \
./Core/Icons/back_icon.d \
./Core/Icons/batt_error_icon.d \
./Core/Icons/batt_warning_icon.d \
./Core/Icons/battery_icon.d \
./Core/Icons/drawers_icon.d \
./Core/Icons/enable_icon.d \
./Core/Icons/play_icon.d \
./Core/Icons/recording_icon.d \
./Core/Icons/thermostat_icon.d \
./Core/Icons/trash_icon.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Icons/%.o Core/Icons/%.su: ../Core/Icons/%.c Core/Icons/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Core/Inc/FSMs/event_manager -I../Core/Fonts -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/lcd -I../Core/Inc/API -I../Core/Inc/API/printf_dbg -I../Core/Inc/API/event_queue -I../Core/Inc/API/time_event -I../Core/Inc/API/ring_buffer -I../Core/Inc/FSMs/ui -I../Core/Inc/API/buttons -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Icons

clean-Core-2f-Icons:
	-$(RM) ./Core/Icons/back_icon.d ./Core/Icons/back_icon.o ./Core/Icons/back_icon.su ./Core/Icons/batt_error_icon.d ./Core/Icons/batt_error_icon.o ./Core/Icons/batt_error_icon.su ./Core/Icons/batt_warning_icon.d ./Core/Icons/batt_warning_icon.o ./Core/Icons/batt_warning_icon.su ./Core/Icons/battery_icon.d ./Core/Icons/battery_icon.o ./Core/Icons/battery_icon.su ./Core/Icons/drawers_icon.d ./Core/Icons/drawers_icon.o ./Core/Icons/drawers_icon.su ./Core/Icons/enable_icon.d ./Core/Icons/enable_icon.o ./Core/Icons/enable_icon.su ./Core/Icons/play_icon.d ./Core/Icons/play_icon.o ./Core/Icons/play_icon.su ./Core/Icons/recording_icon.d ./Core/Icons/recording_icon.o ./Core/Icons/recording_icon.su ./Core/Icons/thermostat_icon.d ./Core/Icons/thermostat_icon.o ./Core/Icons/thermostat_icon.su ./Core/Icons/trash_icon.d ./Core/Icons/trash_icon.o ./Core/Icons/trash_icon.su

.PHONY: clean-Core-2f-Icons

