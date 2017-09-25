################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Bme280.c \
../src/I2C.c \
../src/Timer.c \
../src/Uart.c \
../src/hal_entry.c 

OBJS += \
./src/Bme280.o \
./src/I2C.o \
./src/Timer.o \
./src/Uart.o \
./src/hal_entry.o 

C_DEPS += \
./src/Bme280.d \
./src/I2C.d \
./src/Timer.d \
./src/Uart.d \
./src/hal_entry.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	C:\Renesas\e2_studio\eclipse\../Utilities/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\Users\Kirti\e2_studio\workspace\BME280\src" -I"C:\Users\Kirti\e2_studio\workspace\BME280\src\synergy_gen" -I"C:\Users\Kirti\e2_studio\workspace\BME280\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\Kirti\e2_studio\workspace\BME280\synergy_cfg\ssp_cfg\driver" -I"C:\Users\Kirti\e2_studio\workspace\BME280\synergy\ssp\inc" -I"C:\Users\Kirti\e2_studio\workspace\BME280\synergy\ssp\inc\bsp" -I"C:\Users\Kirti\e2_studio\workspace\BME280\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\Kirti\e2_studio\workspace\BME280\synergy\ssp\inc\driver\api" -I"C:\Users\Kirti\e2_studio\workspace\BME280\synergy\ssp\inc\driver\instances" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


