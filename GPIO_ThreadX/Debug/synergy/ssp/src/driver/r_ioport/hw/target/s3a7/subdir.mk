################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/driver/r_ioport/hw/target/s3a7/hw_ioport_s3a7.c 

OBJS += \
./synergy/ssp/src/driver/r_ioport/hw/target/s3a7/hw_ioport_s3a7.o 

C_DEPS += \
./synergy/ssp/src/driver/r_ioport/hw/target/s3a7/hw_ioport_s3a7.d 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/driver/r_ioport/hw/target/s3a7/%.o: ../synergy/ssp/src/driver/r_ioport/hw/target/s3a7/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	C:\Renesas\e2_studio\eclipse\../Utilities/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\src" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\src\synergy_gen" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy_cfg\ssp_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\inc" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\inc\bsp" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\inc\driver\api" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\inc\driver\instances" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy_cfg\ssp_cfg\framework\el" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\inc\framework\el" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\src\framework\el\tx" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy_cfg\ssp_cfg\framework" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\inc\framework\api" -I"C:\Users\Kirti\e2_studio\workspace\GPIO_ThreadX\synergy\ssp\inc\framework\instances" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


