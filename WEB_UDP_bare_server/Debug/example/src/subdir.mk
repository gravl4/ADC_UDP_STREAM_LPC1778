################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/cr_redlib_heap_fix.c \
../example/src/cr_startup_lpc177x_8x.c \
../example/src/httpd.c \
../example/src/lwip_fs.c \
../example/src/main.c \
../example/src/ringbuffer.c \
../example/src/sysinit.c \
../example/src/tickloop.c 

C_DEPS += \
./example/src/cr_redlib_heap_fix.d \
./example/src/cr_startup_lpc177x_8x.d \
./example/src/httpd.d \
./example/src/lwip_fs.d \
./example/src/main.d \
./example/src/ringbuffer.d \
./example/src/sysinit.d \
./example/src/tickloop.d 

OBJS += \
./example/src/cr_redlib_heap_fix.o \
./example/src/cr_startup_lpc177x_8x.o \
./example/src/httpd.o \
./example/src/lwip_fs.o \
./example/src/main.o \
./example/src/ringbuffer.o \
./example/src/sysinit.o \
./example/src/tickloop.o 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c example/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -D__NEWLIB__ -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_chip_177x_8x/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/lpc_board_ea_devkit_1788/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_UDP_bare_server/example/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_UDP_bare_server/lwip/inc" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_UDP_bare_server/lwip/inc/arch" -I"/home/ygy/eclipse-workspace/ADC_UDP_STREAM_LPC1778/WEB_UDP_bare_server/lwip/src/arch" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-example-2f-src

clean-example-2f-src:
	-$(RM) ./example/src/cr_redlib_heap_fix.d ./example/src/cr_redlib_heap_fix.o ./example/src/cr_startup_lpc177x_8x.d ./example/src/cr_startup_lpc177x_8x.o ./example/src/httpd.d ./example/src/httpd.o ./example/src/lwip_fs.d ./example/src/lwip_fs.o ./example/src/main.d ./example/src/main.o ./example/src/ringbuffer.d ./example/src/ringbuffer.o ./example/src/sysinit.d ./example/src/sysinit.o ./example/src/tickloop.d ./example/src/tickloop.o

.PHONY: clean-example-2f-src

