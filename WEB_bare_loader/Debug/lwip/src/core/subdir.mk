################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/core/altcp.c \
../lwip/src/core/altcp_alloc.c \
../lwip/src/core/altcp_tcp.c \
../lwip/src/core/def.c \
../lwip/src/core/dns.c \
../lwip/src/core/inet_chksum.c \
../lwip/src/core/init.c \
../lwip/src/core/ip.c \
../lwip/src/core/mem.c \
../lwip/src/core/memp.c \
../lwip/src/core/netif.c \
../lwip/src/core/pbuf.c \
../lwip/src/core/raw.c \
../lwip/src/core/stats.c \
../lwip/src/core/sys.c \
../lwip/src/core/tcp.c \
../lwip/src/core/tcp_in.c \
../lwip/src/core/tcp_out.c \
../lwip/src/core/timeouts.c \
../lwip/src/core/udp.c 

C_DEPS += \
./lwip/src/core/altcp.d \
./lwip/src/core/altcp_alloc.d \
./lwip/src/core/altcp_tcp.d \
./lwip/src/core/def.d \
./lwip/src/core/dns.d \
./lwip/src/core/inet_chksum.d \
./lwip/src/core/init.d \
./lwip/src/core/ip.d \
./lwip/src/core/mem.d \
./lwip/src/core/memp.d \
./lwip/src/core/netif.d \
./lwip/src/core/pbuf.d \
./lwip/src/core/raw.d \
./lwip/src/core/stats.d \
./lwip/src/core/sys.d \
./lwip/src/core/tcp.d \
./lwip/src/core/tcp_in.d \
./lwip/src/core/tcp_out.d \
./lwip/src/core/timeouts.d \
./lwip/src/core/udp.d 

OBJS += \
./lwip/src/core/altcp.o \
./lwip/src/core/altcp_alloc.o \
./lwip/src/core/altcp_tcp.o \
./lwip/src/core/def.o \
./lwip/src/core/dns.o \
./lwip/src/core/inet_chksum.o \
./lwip/src/core/init.o \
./lwip/src/core/ip.o \
./lwip/src/core/mem.o \
./lwip/src/core/memp.o \
./lwip/src/core/netif.o \
./lwip/src/core/pbuf.o \
./lwip/src/core/raw.o \
./lwip/src/core/stats.o \
./lwip/src/core/sys.o \
./lwip/src/core/tcp.o \
./lwip/src/core/tcp_in.o \
./lwip/src/core/tcp_out.o \
./lwip/src/core/timeouts.o \
./lwip/src/core/udp.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/core/%.o: ../lwip/src/core/%.c lwip/src/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -D__NEWLIB__ -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_chip_177x_8x/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/example/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/src/arch" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc/lwip" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_bare_loader/lwip/inc/arch" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-src-2f-core

clean-lwip-2f-src-2f-core:
	-$(RM) ./lwip/src/core/altcp.d ./lwip/src/core/altcp.o ./lwip/src/core/altcp_alloc.d ./lwip/src/core/altcp_alloc.o ./lwip/src/core/altcp_tcp.d ./lwip/src/core/altcp_tcp.o ./lwip/src/core/def.d ./lwip/src/core/def.o ./lwip/src/core/dns.d ./lwip/src/core/dns.o ./lwip/src/core/inet_chksum.d ./lwip/src/core/inet_chksum.o ./lwip/src/core/init.d ./lwip/src/core/init.o ./lwip/src/core/ip.d ./lwip/src/core/ip.o ./lwip/src/core/mem.d ./lwip/src/core/mem.o ./lwip/src/core/memp.d ./lwip/src/core/memp.o ./lwip/src/core/netif.d ./lwip/src/core/netif.o ./lwip/src/core/pbuf.d ./lwip/src/core/pbuf.o ./lwip/src/core/raw.d ./lwip/src/core/raw.o ./lwip/src/core/stats.d ./lwip/src/core/stats.o ./lwip/src/core/sys.d ./lwip/src/core/sys.o ./lwip/src/core/tcp.d ./lwip/src/core/tcp.o ./lwip/src/core/tcp_in.d ./lwip/src/core/tcp_in.o ./lwip/src/core/tcp_out.d ./lwip/src/core/tcp_out.o ./lwip/src/core/timeouts.d ./lwip/src/core/timeouts.o ./lwip/src/core/udp.d ./lwip/src/core/udp.o

.PHONY: clean-lwip-2f-src-2f-core

