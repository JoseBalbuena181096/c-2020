################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Self_Balancing.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Self_Balancing.list \

OBJCOPY_HEX += \
Self_Balancing.hex \


# All Target
all: Self_Balancing.elf secondary-outputs

# Tool invocations
Self_Balancing.elf: $(OBJS) $(USER_OBJS) /home/jose/STM32CubeIDE/workspace_1.0.1/Self_Balancing/STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "Self_Balancing.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/jose/STM32CubeIDE/workspace_1.0.1/Self_Balancing/STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Self_Balancing.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Self_Balancing.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Self_Balancing.list"
	@echo 'Finished building: $@'
	@echo ' '

Self_Balancing.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Self_Balancing.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
