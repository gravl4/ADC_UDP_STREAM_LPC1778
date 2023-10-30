################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/netif/bridgeif.c \
../lwip/src/netif/bridgeif_fdb.c \
../lwip/src/netif/ethernet.c \
../lwip/src/netif/lowpan6.c \
../lwip/src/netif/lowpan6_ble.c \
../lwip/src/netif/lowpan6_common.c \
../lwip/src/netif/slipif.c \
../lwip/src/netif/zepif.c 

C_DEPS += \
./lwip/src/netif/bridgeif.d \
./lwip/src/netif/bridgeif_fdb.d \
./lwip/src/netif/ethernet.d \
./lwip/src/netif/lowpan6.d \
./lwip/src/netif/lowpan6_ble.d \
./lwip/src/netif/lowpan6_common.d \
./lwip/src/netif/slipif.d \
./lwip/src/netif/zepif.d 

OBJS += \
./lwip/src/netif/bridgeif.o \
./lwip/src/netif/bridgeif_fdb.o \
./lwip/src/netif/ethernet.o \
./lwip/src/netif/lowpan6.o \
./lwip/src/netif/lowpan6_ble.o \
./lwip/src/netif/lowpan6_common.o \
./lwip/src/netif/slipif.o \
./lwip/src/netif/zepif.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/netif/%.o: ../lwip/src/netif/%.c lwip/src/netif/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -D__NEWLIB__ -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_chip_177x_8x/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_board_ea_devkit_1788/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_UDP_bare_server/example/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_UDP_bare_server/lwip/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_UDP_bare_server/lwip/inc/arch" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_UDP_bare_server/lwip/src/arch" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-src-2f-netif

clean-lwip-2f-src-2f-netif:
	-$(RM) ./lwip/src/netif/bridgeif.d ./lwip/src/netif/bridgeif.o ./lwip/src/netif/bridgeif_fdb.d ./lwip/src/netif/bridgeif_fdb.o ./lwip/src/netif/ethernet.d ./lwip/src/netif/ethernet.o ./lwip/src/netif/lowpan6.d ./lwip/src/netif/lowpan6.o ./lwip/src/netif/lowpan6_ble.d ./lwip/src/netif/lowpan6_ble.o ./lwip/src/netif/lowpan6_common.d ./lwip/src/netif/lowpan6_common.o ./lwip/src/netif/slipif.d ./lwip/src/netif/slipif.o ./lwip/src/netif/zepif.d ./lwip/src/netif/zepif.o

.PHONY: clean-lwip-2f-src-2f-netif

