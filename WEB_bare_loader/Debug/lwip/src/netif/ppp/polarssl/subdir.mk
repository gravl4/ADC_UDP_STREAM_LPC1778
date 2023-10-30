################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/netif/ppp/polarssl/arc4.c \
../lwip/src/netif/ppp/polarssl/des.c \
../lwip/src/netif/ppp/polarssl/md4.c \
../lwip/src/netif/ppp/polarssl/md5.c \
../lwip/src/netif/ppp/polarssl/sha1.c 

C_DEPS += \
./lwip/src/netif/ppp/polarssl/arc4.d \
./lwip/src/netif/ppp/polarssl/des.d \
./lwip/src/netif/ppp/polarssl/md4.d \
./lwip/src/netif/ppp/polarssl/md5.d \
./lwip/src/netif/ppp/polarssl/sha1.d 

OBJS += \
./lwip/src/netif/ppp/polarssl/arc4.o \
./lwip/src/netif/ppp/polarssl/des.o \
./lwip/src/netif/ppp/polarssl/md4.o \
./lwip/src/netif/ppp/polarssl/md5.o \
./lwip/src/netif/ppp/polarssl/sha1.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/netif/ppp/polarssl/%.o: ../lwip/src/netif/ppp/polarssl/%.c lwip/src/netif/ppp/polarssl/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -D__NEWLIB__ -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_chip_177x_8x/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/example/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/src/arch" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc/lwip" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc/arch" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl

clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl:
	-$(RM) ./lwip/src/netif/ppp/polarssl/arc4.d ./lwip/src/netif/ppp/polarssl/arc4.o ./lwip/src/netif/ppp/polarssl/des.d ./lwip/src/netif/ppp/polarssl/des.o ./lwip/src/netif/ppp/polarssl/md4.d ./lwip/src/netif/ppp/polarssl/md4.o ./lwip/src/netif/ppp/polarssl/md5.d ./lwip/src/netif/ppp/polarssl/md5.o ./lwip/src/netif/ppp/polarssl/sha1.d ./lwip/src/netif/ppp/polarssl/sha1.o

.PHONY: clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl

