################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lwip/src/netif/ppp/polarssl/subdir.mk
-include lwip/src/netif/ppp/subdir.mk
-include lwip/src/netif/subdir.mk
-include lwip/src/core/ipv6/subdir.mk
-include lwip/src/core/ipv4/subdir.mk
-include lwip/src/core/subdir.mk
-include lwip/src/arch/subdir.mk
-include lwip/src/api/subdir.mk
-include example/src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := WEB_UDP_bare_server
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: WEB_UDP_bare_server.axf

# Tool invocations
WEB_UDP_bare_server.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_chip_177x_8x/Debug" -L"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_board_ea_devkit_1788/Debug" -Xlinker -Map="WEB_UDP_bare_server.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "my_WEB_UDP_bare_server_Debug.ld" -o "WEB_UDP_bare_server.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) WEB_UDP_bare_server.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "WEB_UDP_bare_server.axf" ; arm-none-eabi-objcopy -v -O binary "WEB_UDP_bare_server.axf" "WEB_UDP_bare_server.bin" ; # checksum -p LPC1788 -d "WEB_UDP_bare_server.bin"
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
