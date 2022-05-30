################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/printf_dbg/itm_dbg.c \
../Core/Src/printf_dbg/printf_dbg.c 

OBJS += \
./Core/Src/printf_dbg/itm_dbg.o \
./Core/Src/printf_dbg/printf_dbg.o 

C_DEPS += \
./Core/Src/printf_dbg/itm_dbg.d \
./Core/Src/printf_dbg/printf_dbg.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/printf_dbg/%.o Core/Src/printf_dbg/%.su: ../Core/Src/printf_dbg/%.c Core/Src/printf_dbg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/API -I../Core/Inc/printf_dbg -I../Core/Inc/time_event -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-printf_dbg

clean-Core-2f-Src-2f-printf_dbg:
	-$(RM) ./Core/Src/printf_dbg/itm_dbg.d ./Core/Src/printf_dbg/itm_dbg.o ./Core/Src/printf_dbg/itm_dbg.su ./Core/Src/printf_dbg/printf_dbg.d ./Core/Src/printf_dbg/printf_dbg.o ./Core/Src/printf_dbg/printf_dbg.su

.PHONY: clean-Core-2f-Src-2f-printf_dbg

