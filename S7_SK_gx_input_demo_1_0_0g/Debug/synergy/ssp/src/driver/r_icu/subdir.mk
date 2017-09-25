################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/driver/r_icu/r_icu.c 

OBJS += \
./synergy/ssp/src/driver/r_icu/r_icu.o 

C_DEPS += \
./synergy/ssp/src/driver/r_icu/r_icu.d 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/driver/r_icu/%.o: ../synergy/ssp/src/driver/r_icu/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	C:\Renesas\e2_studio\eclipse\../Utilities/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\ssp_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\bsp" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\driver\api" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\driver\instances" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\src" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\src\synergy_gen" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\ssp_cfg\framework\el" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\framework\el" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\src\framework\el\tx" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\ssp_cfg\framework" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\framework\api" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\framework\instances" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\framework\tes" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\framework" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\ssp_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\bsp" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\driver\api" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\driver\instances" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\src" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\src\synergy_gen" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\ssp_cfg\framework\el" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\framework\el" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\src\framework\el\tx" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\ssp_cfg\framework" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\framework\api" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\framework\instances" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy\ssp\inc\framework\tes" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\S7_SK_gx_input_demo_1_0_0g\synergy_cfg\framework" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


