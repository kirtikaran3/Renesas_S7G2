################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c.c \
../synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c_ft5x06.c \
../synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c_sx8654.c 

OBJS += \
./synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c.o \
./synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c_ft5x06.o \
./synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c_sx8654.o 

C_DEPS += \
./synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c.d \
./synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c_ft5x06.d \
./synergy/ssp/src/framework/sf_touch_panel_i2c/sf_touch_panel_i2c_sx8654.d 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/framework/sf_touch_panel_i2c/%.o: ../synergy/ssp/src/framework/sf_touch_panel_i2c/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	C:\Renesas\e2_studio\eclipse\../Utilities/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy_cfg\ssp_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc\bsp" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc\driver\api" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc\driver\instances" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\src" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\src\synergy_gen" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy_cfg\ssp_cfg\framework" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc\framework\api" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc\framework\instances" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc\framework\tes" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy_cfg\ssp_cfg\framework\el" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\inc\framework\el" -I"C:\Users\Kirti\e2_studio\workspace\GUI_APP\GUIAppADC\synergy\ssp\src\framework\el\tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


