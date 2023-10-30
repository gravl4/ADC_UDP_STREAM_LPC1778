################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/src/FreeRTOSCommonHooks.c \
../freertos/src/heap_3.c \
../freertos/src/list.c \
../freertos/src/port.c \
../freertos/src/queue.c \
../freertos/src/tasks.c \
../freertos/src/timers.c 

OBJS += \
./freertos/src/FreeRTOSCommonHooks.o \
./freertos/src/heap_3.o \
./freertos/src/list.o \
./freertos/src/port.o \
./freertos/src/queue.o \
./freertos/src/tasks.o \
./freertos/src/timers.o 

C_DEPS += \
./freertos/src/FreeRTOSCommonHooks.d \
./freertos/src/heap_3.d \
./freertos/src/list.d \
./freertos/src/port.d \
./freertos/src/queue.d \
./freertos/src/tasks.d \
./freertos/src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/src/%.o: ../freertos/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -I"C:\_freelance\Netping\1778_web\lpc_chip_177x_8x\inc" -I"C:\_freelance\Netping\1778_web\lpc_board_ea_devkit_1788\inc" -I"C:\_freelance\Netping\DRBF_52\WEB_loader_LPC1788\example\inc" -I"C:\_freelance\Netping\DRBF_52\WEB_loader_LPC1788\lwip\inc" -I"C:\_freelance\Netping\DRBF_52\WEB_loader_LPC1788\lwip\inc\ipv4" -I"C:\_freelance\Netping\DRBF_52\WEB_loader_LPC1788\freertos\inc" -I"C:\_freelance\Netping\DRBF_52\WEB_loader_LPC1788\lwip\inc\arch" -I"C:\_freelance\Netping\DRBF_52\WEB_loader_LPC1788\lwip\src\arch" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


