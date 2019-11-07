################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
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
01Menghitung_LAP_Balap.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
01Menghitung_LAP_Balap.list \

OBJCOPY_HEX += \
01Menghitung_LAP_Balap.hex \


# All Target
all: 01Menghitung_LAP_Balap.elf secondary-outputs

# Tool invocations
01Menghitung_LAP_Balap.elf: $(OBJS) $(USER_OBJS) ../STM32F103C8Tx_FLASH.ld
	arm-none-eabi-gcc -o "01Menghitung_LAP_Balap.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"../STM32F103C8Tx_FLASH.ld" --specs=nosys.specs -Wl,-Map="01Menghitung_LAP_Balap.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

01Menghitung_LAP_Balap.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "01Menghitung_LAP_Balap.list"
	@echo 'Finished building: $@'
	@echo ' '

01Menghitung_LAP_Balap.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "01Menghitung_LAP_Balap.hex"
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
