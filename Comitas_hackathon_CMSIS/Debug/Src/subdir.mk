################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/TH02.c \
../Src/UART_for_PC.c \
../Src/common.c \
../Src/ds18b20.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/TH02.o \
./Src/UART_for_PC.o \
./Src/common.o \
./Src/ds18b20.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/TH02.d \
./Src/UART_for_PC.d \
./Src/common.d \
./Src/ds18b20.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -c -I"C:/Users/denlo/Documents/stm32_projects/Comitas_hackathon/Comitas_hackathon_CMSIS/CMSIS/Inc" -I"C:/Users/denlo/Documents/stm32_projects/Comitas_hackathon/Comitas_hackathon_CMSIS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/TH02.cyclo ./Src/TH02.d ./Src/TH02.o ./Src/TH02.su ./Src/UART_for_PC.cyclo ./Src/UART_for_PC.d ./Src/UART_for_PC.o ./Src/UART_for_PC.su ./Src/common.cyclo ./Src/common.d ./Src/common.o ./Src/common.su ./Src/ds18b20.cyclo ./Src/ds18b20.d ./Src/ds18b20.o ./Src/ds18b20.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

