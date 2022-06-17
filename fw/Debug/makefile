################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/lcd/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/FSMs/ui/subdir.mk
-include Core/Src/FSMs/temperature/subdir.mk
-include Core/Src/FSMs/power/subdir.mk
-include Core/Src/FSMs/feeder/subdir.mk
-include Core/Src/FSMs/event_manager/subdir.mk
-include Core/Src/FSMs/drawer/subdir.mk
-include Core/Src/API/time_event/subdir.mk
-include Core/Src/API/ring_buffer/subdir.mk
-include Core/Src/API/printf_dbg/subdir.mk
-include Core/Src/API/event_queue/subdir.mk
-include Core/Src/API/buttons/subdir.mk
-include Core/Src/subdir.mk
-include Core/Icons/subdir.mk
-include Core/Fonts/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := pet_feeder
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
pet_feeder.elf \

MAP_FILES += \
pet_feeder.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
pet_feeder.list \


# All Target
all: main-build

# Main-build Target
main-build: pet_feeder.elf secondary-outputs

# Tool invocations
pet_feeder.elf pet_feeder.map: $(OBJS) $(USER_OBJS) D:\Documents\GitHub\Ali_pet_feeder\fw\STM32F103R8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "pet_feeder.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\Documents\GitHub\Ali_pet_feeder\fw\STM32F103R8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="pet_feeder.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

pet_feeder.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "pet_feeder.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout pet_feeder.elf pet_feeder.list pet_feeder.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
