################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include synergy/ssp/src/driver/r_sci_uart/subdir.mk
-include synergy/ssp/src/driver/r_sci_common/subdir.mk
-include synergy/ssp/src/driver/r_ioport/hw/target/s7g2/subdir.mk
-include synergy/ssp/src/driver/r_ioport/hw/target/s3a7/subdir.mk
-include synergy/ssp/src/driver/r_ioport/subdir.mk
-include synergy/ssp/src/driver/r_elc/subdir.mk
-include synergy/ssp/src/driver/r_cgc/hw/target/subdir.mk
-include synergy/ssp/src/driver/r_cgc/hw/common/subdir.mk
-include synergy/ssp/src/driver/r_cgc/hw/subdir.mk
-include synergy/ssp/src/driver/r_cgc/subdir.mk
-include synergy/ssp/src/bsp/mcu/s7g2/subdir.mk
-include synergy/ssp/src/bsp/mcu/all/subdir.mk
-include synergy/ssp/src/bsp/cmsis/Device/RENESAS/S7G2/Source/subdir.mk
-include synergy/ssp/src/bsp/cmsis/Device/RENESAS/S3A7/Source/subdir.mk
-include synergy/ssp/src/bsp/board/s7g2_sk/subdir.mk
-include src/synergy_gen/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(DSP_DEPS)),)
-include $(DSP_DEPS)
endif
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif
ifneq ($(strip $(INC_DEPS)),)
-include $(INC_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
UART_Input.srec \

SECONDARY_SIZE += \
UART_Input.siz \


# All Target
all: UART_Input.elf secondary-outputs

# Tool invocations
UART_Input.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	$(file > UART_Input.elf.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -T C:\\Users\\Kirti\\e2_studio\\workspace\\UART_Input\\script\\S7G2.ld -Xlinker --gc-sections -L "C:\\Users\\Kirti\\e2_studio\\workspace\\UART_Input\\synergy\\ssp\\src\\bsp\\cmsis\\DSP_Lib\\cm4_gcc" -L "C:\\Users\\Kirti\\e2_studio\\workspace\\UART_Input\\synergy\\ssp\\src\\framework\\el\\tx\\cm4_gcc" -Wl,-Map,"UART_Input.map" --specs=rdimon.specs -o "UART_Input.elf" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group)
	arm-none-eabi-gcc @"UART_Input.elf.in"
	@echo 'Finished building target: $@'
	@echo ' '

UART_Input.srec: UART_Input.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O srec "UART_Input.elf"  "UART_Input.srec"
	@echo 'Finished building: $@'
	@echo ' '

UART_Input.siz: UART_Input.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "UART_Input.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
ifneq ($(strip $(SRC_DEPS)),)
	-$(file > clean_SRC_DEPS.tmp, $(SRC_DEPS))
	-@ xargs -s 6000 -t -a clean_SRC_DEPS.tmp $(RM)
	-@$(RM) clean_SRC_DEPS.tmp
endif
ifneq ($(strip $(DSP_DEPS)),)
	-$(file > clean_DSP_DEPS.tmp, $(DSP_DEPS))
	-@ xargs -s 6000 -t -a clean_DSP_DEPS.tmp $(RM)
	-@$(RM) clean_DSP_DEPS.tmp
endif
ifneq ($(strip $(PP_DEPS)),)
	-$(file > clean_PP_DEPS.tmp, $(PP_DEPS))
	-@ xargs -s 6000 -t -a clean_PP_DEPS.tmp $(RM)
	-@$(RM) clean_PP_DEPS.tmp
endif
ifneq ($(strip $(OBJS)),)
	-$(file > clean_OBJS.tmp, $(OBJS))
	-@ xargs -s 6000 -t -a clean_OBJS.tmp $(RM)
	-@$(RM) clean_OBJS.tmp
endif
ifneq ($(strip $(SECONDARY_FLASH)),)
	-$(file > clean_SECONDARY_FLASH.tmp, $(SECONDARY_FLASH))
	-@ xargs -s 6000 -t -a clean_SECONDARY_FLASH.tmp $(RM)
	-@$(RM) clean_SECONDARY_FLASH.tmp
endif
ifneq ($(strip $(SECONDARY_SIZE)),)
	-$(file > clean_SECONDARY_SIZE.tmp, $(SECONDARY_SIZE))
	-@ xargs -s 6000 -t -a clean_SECONDARY_SIZE.tmp $(RM)
	-@$(RM) clean_SECONDARY_SIZE.tmp
endif
ifneq ($(strip $(ASM_DEPS)),)
	-$(file > clean_ASM_DEPS.tmp, $(ASM_DEPS))
	-@ xargs -s 6000 -t -a clean_ASM_DEPS.tmp $(RM)
	-@$(RM) clean_ASM_DEPS.tmp
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
	-$(file > clean_S_UPPER_DEPS.tmp, $(S_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_S_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_S_UPPER_DEPS.tmp
endif
ifneq ($(strip $(P_DEPS)),)
	-$(file > clean_P_DEPS.tmp, $(P_DEPS))
	-@ xargs -s 6000 -t -a clean_P_DEPS.tmp $(RM)
	-@$(RM) clean_P_DEPS.tmp
endif
ifneq ($(strip $(INC_DEPS)),)
	-$(file > clean_INC_DEPS.tmp, $(INC_DEPS))
	-@ xargs -s 6000 -t -a clean_INC_DEPS.tmp $(RM)
	-@$(RM) clean_INC_DEPS.tmp
endif
ifneq ($(strip $(C_DEPS)),)
	-$(file > clean_C_DEPS.tmp, $(C_DEPS))
	-@ xargs -s 6000 -t -a clean_C_DEPS.tmp $(RM)
	-@$(RM) clean_C_DEPS.tmp
endif
	-$(RM)  UART_Input.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
