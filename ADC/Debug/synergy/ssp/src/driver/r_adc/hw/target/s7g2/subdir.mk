################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/driver/r_adc/hw/target/s7g2/hw_adc_private_s7g2.c \
../synergy/ssp/src/driver/r_adc/hw/target/s7g2/r_adc_private_s7g2.c 

OBJS += \
./synergy/ssp/src/driver/r_adc/hw/target/s7g2/hw_adc_private_s7g2.o \
./synergy/ssp/src/driver/r_adc/hw/target/s7g2/r_adc_private_s7g2.o 

C_DEPS += \
./synergy/ssp/src/driver/r_adc/hw/target/s7g2/hw_adc_private_s7g2.d \
./synergy/ssp/src/driver/r_adc/hw/target/s7g2/r_adc_private_s7g2.d 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/driver/r_adc/hw/target/s7g2/%.o: ../synergy/ssp/src/driver/r_adc/hw/target/s7g2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	C:\Renesas\e2_studio\eclipse\../Utilities/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\Users\Kirti\e2_studio\workspace\ADC\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\Kirti\e2_studio\workspace\ADC\synergy_cfg\ssp_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\ADC\synergy\ssp\inc" -I"C:\Users\Kirti\e2_studio\workspace\ADC\synergy\ssp\inc\bsp" -I"C:\Users\Kirti\e2_studio\workspace\ADC\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\Kirti\e2_studio\workspace\ADC\synergy\ssp\inc\driver\api" -I"C:\Users\Kirti\e2_studio\workspace\ADC\synergy\ssp\inc\driver\instances" -I"C:\Users\Kirti\e2_studio\workspace\ADC\src" -I"C:\Users\Kirti\e2_studio\workspace\ADC\src\synergy_gen" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


