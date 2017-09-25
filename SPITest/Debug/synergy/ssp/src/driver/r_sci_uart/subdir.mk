################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/driver/r_sci_uart/r_sci_uart.c 

OBJS += \
./synergy/ssp/src/driver/r_sci_uart/r_sci_uart.o 

C_DEPS += \
./synergy/ssp/src/driver/r_sci_uart/r_sci_uart.d 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/driver/r_sci_uart/%.o: ../synergy/ssp/src/driver/r_sci_uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	C:\Renesas\e2_studio\eclipse\../Utilities/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\Users\Kirti\e2_studio\workspace\SPITest\src" -I"C:\Users\Kirti\e2_studio\workspace\SPITest\src\synergy_gen" -I"C:\Users\Kirti\e2_studio\workspace\SPITest\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\Kirti\e2_studio\workspace\SPITest\synergy_cfg\ssp_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\SPITest\synergy\ssp\inc" -I"C:\Users\Kirti\e2_studio\workspace\SPITest\synergy\ssp\inc\bsp" -I"C:\Users\Kirti\e2_studio\workspace\SPITest\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\Kirti\e2_studio\workspace\SPITest\synergy\ssp\inc\driver\api" -I"C:\Users\Kirti\e2_studio\workspace\SPITest\synergy\ssp\inc\driver\instances" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


