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
balancing.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
balancing.list \

OBJCOPY_HEX += \
balancing.hex \

OBJCOPY_BIN += \
balancing.bin \

OBJCOPY_VERILOG += \
balancing.verilog \

OBJCOPY_SREC += \
balancing.srec \

OBJCOPY_SYMBOLSREC += \
balancing.symbolsrec \


# All Target
all: balancing.elf secondary-outputs

# Tool invocations
balancing.elf: $(OBJS) $(USER_OBJS) /home/jose/STM32CubeIDE/workspace_1.0.1/balancing/STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "balancing.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/jose/STM32CubeIDE/workspace_1.0.1/balancing/STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="balancing.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

balancing.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "balancing.list"
	@echo 'Finished building: $@'
	@echo ' '

balancing.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "balancing.hex"
	@echo 'Finished building: $@'
	@echo ' '

balancing.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "balancing.bin"
	@echo 'Finished building: $@'
	@echo ' '

balancing.verilog: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O verilog $(EXECUTABLES) "balancing.verilog"
	@echo 'Finished building: $@'
	@echo ' '

balancing.srec: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O srec $(EXECUTABLES) "balancing.srec"
	@echo 'Finished building: $@'
	@echo ' '

balancing.symbolsrec: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O symbolsrec $(EXECUTABLES) "balancing.symbolsrec"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN) $(OBJCOPY_VERILOG) $(OBJCOPY_SREC) $(OBJCOPY_SYMBOLSREC)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
