################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/Src/system_stm32f1xx.c 

OBJS += \
./CMSIS/Src/system_stm32f1xx.o 

C_DEPS += \
./CMSIS/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/Src/%.o CMSIS/Src/%.su CMSIS/Src/%.cyclo: ../CMSIS/Src/%.c CMSIS/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -c -I"C:/Users/denlo/Documents/stm32_projects/Comitas_hackathon_CMSIS/CMSIS/Inc" -I"C:/Users/denlo/Documents/stm32_projects/Comitas_hackathon_CMSIS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CMSIS-2f-Src

clean-CMSIS-2f-Src:
	-$(RM) ./CMSIS/Src/system_stm32f1xx.cyclo ./CMSIS/Src/system_stm32f1xx.d ./CMSIS/Src/system_stm32f1xx.o ./CMSIS/Src/system_stm32f1xx.su

.PHONY: clean-CMSIS-2f-Src

