################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/arch/lpc17xx_40xx_emac.c \
../lwip/src/arch/lpc17xx_40xx_systick_arch.c \
../lwip/src/arch/lpc_debug.c 

C_DEPS += \
./lwip/src/arch/lpc17xx_40xx_emac.d \
./lwip/src/arch/lpc17xx_40xx_systick_arch.d \
./lwip/src/arch/lpc_debug.d 

OBJS += \
./lwip/src/arch/lpc17xx_40xx_emac.o \
./lwip/src/arch/lpc17xx_40xx_systick_arch.o \
./lwip/src/arch/lpc_debug.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/arch/%.o: ../lwip/src/arch/%.c lwip/src/arch/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -D__NEWLIB__ -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_chip_177x_8x/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/example/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/src/arch" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc/lwip" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc/arch" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-src-2f-arch

clean-lwip-2f-src-2f-arch:
	-$(RM) ./lwip/src/arch/lpc17xx_40xx_emac.d ./lwip/src/arch/lpc17xx_40xx_emac.o ./lwip/src/arch/lpc17xx_40xx_systick_arch.d ./lwip/src/arch/lpc17xx_40xx_systick_arch.o ./lwip/src/arch/lpc_debug.d ./lwip/src/arch/lpc_debug.o

.PHONY: clean-lwip-2f-src-2f-arch

