################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/circular_buffer.c \
../source/i2c.c \
../source/main.c \
../source/mtb.c \
../source/pwm.c \
../source/semihost_hardfault.c \
../source/sht21.c \
../source/sysclock.c \
../source/systick.c \
../source/test.c \
../source/tsl2561.c \
../source/uart.c 

OBJS += \
./source/circular_buffer.o \
./source/i2c.o \
./source/main.o \
./source/mtb.o \
./source/pwm.o \
./source/semihost_hardfault.o \
./source/sht21.o \
./source/sysclock.o \
./source/systick.o \
./source/test.o \
./source/tsl2561.o \
./source/uart.o 

C_DEPS += \
./source/circular_buffer.d \
./source/i2c.d \
./source/main.d \
./source/mtb.d \
./source/pwm.d \
./source/semihost_hardfault.d \
./source/sht21.d \
./source/sysclock.d \
./source/systick.d \
./source/test.d \
./source/tsl2561.d \
./source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\hwsin\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Sensors_I2C\board" -I"C:\Users\hwsin\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Sensors_I2C\source" -I"C:\Users\hwsin\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Sensors_I2C" -I"C:\Users\hwsin\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Sensors_I2C\drivers" -I"C:\Users\hwsin\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Sensors_I2C\CMSIS" -I"C:\Users\hwsin\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Sensors_I2C\utilities" -I"C:\Users\hwsin\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Sensors_I2C\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


