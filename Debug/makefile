################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Libraries/XMCLib/src/subdir.mk
-include Libraries/Newlib/subdir.mk
-include Dave/Generated/SYSTIMER/subdir.mk
-include Dave/Generated/PWM_CCU8/subdir.mk
-include Dave/Generated/GLOBAL_CCU8/subdir.mk
-include Dave/Generated/EVENT_DETECTOR/subdir.mk
-include Dave/Generated/CPU_CTRL_XMC4/subdir.mk
-include Dave/Generated/CLOCK_XMC4/subdir.mk
-include Dave/Generated/subdir.mk
-include subdir.mk
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

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
PWM_from_ex.hex \

SECONDARY_LIST += \
PWM_from_ex.lst \

SECONDARY_SIZE += \
PWM_from_ex.siz \


LIST_FILES = $(addsuffix .o.lst, $(basename $(OBJS))) 
MAP_FILE = PWM_from_ex.map

# All Target
all: PWM_from_ex.elf secondary-outputs

# Tool invocations
PWM_from_ex.elf: $(OBJS) ../linker_script.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM-GCC C Linker'
	$(file >objects.rsp,$(OBJS))
	"D:\DAVE-4.1.2\eclipse\ARM-GCC-49/bin/arm-none-eabi-gcc" -T"../linker_script.ld" -nostartfiles -Xlinker --gc-sections -specs=nano.specs -Wl,-Map,$(MAP_FILE) -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mthumb -g -gdwarf-2 -o "PWM_from_ex.elf" "@objects.rsp" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo.
PWM_from_ex.hex: PWM_from_ex.elf
	@echo 'Invoking: ARM-GCC Create Flash Image'
	"D:\DAVE-4.1.2\eclipse\ARM-GCC-49/bin/arm-none-eabi-objcopy" -O ihex "PWM_from_ex.elf"  "PWM_from_ex.hex"
	@echo 'Finished building: $@'
	@echo.
PWM_from_ex.lst: PWM_from_ex.elf
	@echo 'Invoking: ARM-GCC Create Listing'
	"D:\DAVE-4.1.2\eclipse\ARM-GCC-49/bin/arm-none-eabi-objdump" -h -S "PWM_from_ex.elf" > "PWM_from_ex.lst"
	@echo 'Finished building: $@'
	@echo.
PWM_from_ex.siz: PWM_from_ex.elf
	@echo 'Invoking: ARM-GCC Print Size'
	"D:\DAVE-4.1.2\eclipse\ARM-GCC-49/bin/arm-none-eabi-size"  --format=berkeley "PWM_from_ex.elf"
	@echo 'Finished building: $@'
	@echo.
# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ARM_GCC_FOR_XMC_LINKER_OUTPUT_TYPE_OUTPUTS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) PWM_from_ex.elf $(LIST_FILES) $(MAP_FILE) objects.rsp
	-@echo.
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
