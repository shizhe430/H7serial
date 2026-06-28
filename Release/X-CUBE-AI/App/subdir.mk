################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../X-CUBE-AI/App/water_detect.c \
../X-CUBE-AI/App/water_detect_data.c \
../X-CUBE-AI/App/water_detect_data_params.c \
../X-CUBE-AI/App/waterlevel.c \
../X-CUBE-AI/App/waterlevel_data.c \
../X-CUBE-AI/App/waterlevel_data_params.c 

OBJS += \
./X-CUBE-AI/App/water_detect.o \
./X-CUBE-AI/App/water_detect_data.o \
./X-CUBE-AI/App/water_detect_data_params.o \
./X-CUBE-AI/App/waterlevel.o \
./X-CUBE-AI/App/waterlevel_data.o \
./X-CUBE-AI/App/waterlevel_data_params.o 

C_DEPS += \
./X-CUBE-AI/App/water_detect.d \
./X-CUBE-AI/App/water_detect_data.d \
./X-CUBE-AI/App/water_detect_data_params.d \
./X-CUBE-AI/App/waterlevel.d \
./X-CUBE-AI/App/waterlevel_data.d \
./X-CUBE-AI/App/waterlevel_data_params.d 


# Each subdirectory must supply rules for building sources it contributes
X-CUBE-AI/App/%.o X-CUBE-AI/App/%.su X-CUBE-AI/App/%.cyclo: ../X-CUBE-AI/App/%.c X-CUBE-AI/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/AI/Inc -I../X-CUBE-AI/App -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-X-2d-CUBE-2d-AI-2f-App

clean-X-2d-CUBE-2d-AI-2f-App:
	-$(RM) ./X-CUBE-AI/App/water_detect.cyclo ./X-CUBE-AI/App/water_detect.d ./X-CUBE-AI/App/water_detect.o ./X-CUBE-AI/App/water_detect.su ./X-CUBE-AI/App/water_detect_data.cyclo ./X-CUBE-AI/App/water_detect_data.d ./X-CUBE-AI/App/water_detect_data.o ./X-CUBE-AI/App/water_detect_data.su ./X-CUBE-AI/App/water_detect_data_params.cyclo ./X-CUBE-AI/App/water_detect_data_params.d ./X-CUBE-AI/App/water_detect_data_params.o ./X-CUBE-AI/App/water_detect_data_params.su ./X-CUBE-AI/App/waterlevel.cyclo ./X-CUBE-AI/App/waterlevel.d ./X-CUBE-AI/App/waterlevel.o ./X-CUBE-AI/App/waterlevel.su ./X-CUBE-AI/App/waterlevel_data.cyclo ./X-CUBE-AI/App/waterlevel_data.d ./X-CUBE-AI/App/waterlevel_data.o ./X-CUBE-AI/App/waterlevel_data.su ./X-CUBE-AI/App/waterlevel_data_params.cyclo ./X-CUBE-AI/App/waterlevel_data_params.d ./X-CUBE-AI/App/waterlevel_data_params.o ./X-CUBE-AI/App/waterlevel_data_params.su

.PHONY: clean-X-2d-CUBE-2d-AI-2f-App

