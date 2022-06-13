################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Fonts/font12.c \
../Core/Fonts/font150.c \
../Core/Fonts/font16.c \
../Core/Fonts/font20.c \
../Core/Fonts/font24.c \
../Core/Fonts/font8.c 

OBJS += \
./Core/Fonts/font12.o \
./Core/Fonts/font150.o \
./Core/Fonts/font16.o \
./Core/Fonts/font20.o \
./Core/Fonts/font24.o \
./Core/Fonts/font8.o 

C_DEPS += \
./Core/Fonts/font12.d \
./Core/Fonts/font150.d \
./Core/Fonts/font16.d \
./Core/Fonts/font20.d \
./Core/Fonts/font24.d \
./Core/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Fonts/%.o Core/Fonts/%.su: ../Core/Fonts/%.c Core/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Core/Inc/FSMs/event_manager -I../Core/Fonts -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/lcd -I../Core/Inc/API -I../Core/Inc/API/printf_dbg -I../Core/Inc/API/event_queue -I../Core/Inc/API/time_event -I../Core/Inc/API/ring_buffer -I../Core/Inc/FSMs/ui -I../Core/Inc/API/buttons -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Fonts

clean-Core-2f-Fonts:
	-$(RM) ./Core/Fonts/font12.d ./Core/Fonts/font12.o ./Core/Fonts/font12.su ./Core/Fonts/font150.d ./Core/Fonts/font150.o ./Core/Fonts/font150.su ./Core/Fonts/font16.d ./Core/Fonts/font16.o ./Core/Fonts/font16.su ./Core/Fonts/font20.d ./Core/Fonts/font20.o ./Core/Fonts/font20.su ./Core/Fonts/font24.d ./Core/Fonts/font24.o ./Core/Fonts/font24.su ./Core/Fonts/font8.d ./Core/Fonts/font8.o ./Core/Fonts/font8.su

.PHONY: clean-Core-2f-Fonts

