################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/apps/http/makefsdata/makefsdata.c 

OBJS += \
./lwip/src/apps/http/makefsdata/makefsdata.o 

C_DEPS += \
./lwip/src/apps/http/makefsdata/makefsdata.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/apps/http/makefsdata/%.o: ../lwip/src/apps/http/makefsdata/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -D__NEWLIB__ -I"C:\_freelance\Netping\1778_web\lpc_chip_177x_8x\inc" -I"C:\_freelance\Netping\1778_web\lpc_board_ea_devkit_1788\inc" -I"C:\_freelance\Netping\DRBF_52\WEB_UDP_bare_server\example\inc" -I"C:\_freelance\Netping\DRBF_52\WEB_UDP_bare_server\lwip\inc" -I"C:\_freelance\Netping\DRBF_52\WEB_UDP_bare_server\lwip\inc\arch" -I"C:\_freelance\Netping\DRBF_52\WEB_UDP_bare_server\lwip\src\arch" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


