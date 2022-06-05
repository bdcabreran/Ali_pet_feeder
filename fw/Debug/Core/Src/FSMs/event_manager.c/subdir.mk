################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/FSMs/event_manager.c/evt_mgmt_fsm.c 

OBJS += \
./Core/Src/FSMs/event_manager.c/evt_mgmt_fsm.o 

C_DEPS += \
./Core/Src/FSMs/event_manager.c/evt_mgmt_fsm.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/FSMs/event_manager.c/%.o Core/Src/FSMs/event_manager.c/%.su: ../Core/Src/FSMs/event_manager.c/%.c Core/Src/FSMs/event_manager.c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/FSMs/event_manager -I../Core/Inc/API -I../Core/Inc/API/printf_dbg -I../Core/Inc/API/event_queue -I../Core/Inc/API/time_event -I../Core/Inc/API/ring_buffer -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-FSMs-2f-event_manager-2e-c

clean-Core-2f-Src-2f-FSMs-2f-event_manager-2e-c:
	-$(RM) ./Core/Src/FSMs/event_manager.c/evt_mgmt_fsm.d ./Core/Src/FSMs/event_manager.c/evt_mgmt_fsm.o ./Core/Src/FSMs/event_manager.c/evt_mgmt_fsm.su

.PHONY: clean-Core-2f-Src-2f-FSMs-2f-event_manager-2e-c

