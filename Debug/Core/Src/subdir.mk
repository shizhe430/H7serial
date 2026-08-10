################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/as608.c \
../Core/Src/background_img.c \
../Core/Src/camera_app.c \
../Core/Src/camera_light.c \
../Core/Src/character_library.c \
../Core/Src/ctiic.c \
../Core/Src/dcmi.c \
../Core/Src/delay.c \
../Core/Src/dma.c \
../Core/Src/environment_sensors.c \
../Core/Src/face_ai.c \
../Core/Src/fmc.c \
../Core/Src/font.c \
../Core/Src/geren_logo.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/id_logo.c \
../Core/Src/jianyi_logo.c \
../Core/Src/jpeg_decode.c \
../Core/Src/jpeg_stream.c \
../Core/Src/lcd.c \
../Core/Src/lcd_ui.c \
../Core/Src/main.c \
../Core/Src/oled_1.c \
../Core/Src/oled_status.c \
../Core/Src/oledfont.c \
../Core/Src/openmv_face.c \
../Core/Src/openmv_frontalface_data.c \
../Core/Src/ov2640.c \
../Core/Src/ov2640_sccb.c \
../Core/Src/pump.c \
../Core/Src/quadspi.c \
../Core/Src/shuiwei_logo.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32h7xx.c \
../Core/Src/tjpgd.c \
../Core/Src/touch.c \
../Core/Src/usart.c \
../Core/Src/voice_asr.c \
../Core/Src/water_logo.c \
../Core/Src/wendu_logo.c \
../Core/Src/yinshui_logo.c \
../Core/Src/yinshuiji_logo.c \
../Core/Src/yue_logo.c

OBJS += \
./Core/Src/as608.o \
./Core/Src/background_img.o \
./Core/Src/camera_app.o \
./Core/Src/camera_light.o \
./Core/Src/character_library.o \
./Core/Src/ctiic.o \
./Core/Src/dcmi.o \
./Core/Src/delay.o \
./Core/Src/dma.o \
./Core/Src/environment_sensors.o \
./Core/Src/face_ai.o \
./Core/Src/fmc.o \
./Core/Src/font.o \
./Core/Src/geren_logo.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/id_logo.o \
./Core/Src/jianyi_logo.o \
./Core/Src/jpeg_decode.o \
./Core/Src/jpeg_stream.o \
./Core/Src/lcd.o \
./Core/Src/lcd_ui.o \
./Core/Src/main.o \
./Core/Src/oled_1.o \
./Core/Src/oled_status.o \
./Core/Src/oledfont.o \
./Core/Src/openmv_face.o \
./Core/Src/openmv_frontalface_data.o \
./Core/Src/ov2640.o \
./Core/Src/ov2640_sccb.o \
./Core/Src/pump.o \
./Core/Src/quadspi.o \
./Core/Src/shuiwei_logo.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32h7xx.o \
./Core/Src/tjpgd.o \
./Core/Src/touch.o \
./Core/Src/usart.o \
./Core/Src/voice_asr.o \
./Core/Src/water_logo.o \
./Core/Src/wendu_logo.o \
./Core/Src/yinshui_logo.o \
./Core/Src/yinshuiji_logo.o \
./Core/Src/yue_logo.o

C_DEPS += \
./Core/Src/as608.d \
./Core/Src/background_img.d \
./Core/Src/camera_app.d \
./Core/Src/camera_light.d \
./Core/Src/character_library.d \
./Core/Src/ctiic.d \
./Core/Src/dcmi.d \
./Core/Src/delay.d \
./Core/Src/dma.d \
./Core/Src/environment_sensors.d \
./Core/Src/face_ai.d \
./Core/Src/fmc.d \
./Core/Src/font.d \
./Core/Src/geren_logo.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/id_logo.d \
./Core/Src/jianyi_logo.d \
./Core/Src/jpeg_decode.d \
./Core/Src/jpeg_stream.d \
./Core/Src/lcd.d \
./Core/Src/lcd_ui.d \
./Core/Src/main.d \
./Core/Src/oled_1.d \
./Core/Src/oled_status.d \
./Core/Src/oledfont.d \
./Core/Src/openmv_face.d \
./Core/Src/openmv_frontalface_data.d \
./Core/Src/ov2640.d \
./Core/Src/ov2640_sccb.d \
./Core/Src/pump.d \
./Core/Src/quadspi.d \
./Core/Src/shuiwei_logo.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32h7xx.d \
./Core/Src/tjpgd.d \
./Core/Src/touch.d \
./Core/Src/usart.d \
./Core/Src/voice_asr.d \
./Core/Src/water_logo.d \
./Core/Src/wendu_logo.d \
./Core/Src/yinshui_logo.d \
./Core/Src/yinshuiji_logo.d \
./Core/Src/yue_logo.d


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/AI/Inc -I../X-CUBE-AI/App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/camera_app.cyclo ./Core/Src/camera_app.d ./Core/Src/camera_app.o ./Core/Src/camera_app.su ./Core/Src/dcmi.cyclo ./Core/Src/dcmi.d ./Core/Src/dcmi.o ./Core/Src/dcmi.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/jpeg_decode.cyclo ./Core/Src/jpeg_decode.d ./Core/Src/jpeg_decode.o ./Core/Src/jpeg_decode.su ./Core/Src/jpeg_stream.cyclo ./Core/Src/jpeg_stream.d ./Core/Src/jpeg_stream.o ./Core/Src/jpeg_stream.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/ov2640.cyclo ./Core/Src/ov2640.d ./Core/Src/ov2640.o ./Core/Src/ov2640.su ./Core/Src/ov2640_sccb.cyclo ./Core/Src/ov2640_sccb.d ./Core/Src/ov2640_sccb.o ./Core/Src/ov2640_sccb.su ./Core/Src/pump.cyclo ./Core/Src/pump.d ./Core/Src/pump.o ./Core/Src/pump.su ./Core/Src/stm32h7xx_hal_msp.cyclo ./Core/Src/stm32h7xx_hal_msp.d ./Core/Src/stm32h7xx_hal_msp.o ./Core/Src/stm32h7xx_hal_msp.su ./Core/Src/stm32h7xx_it.cyclo ./Core/Src/stm32h7xx_it.d ./Core/Src/stm32h7xx_it.o ./Core/Src/stm32h7xx_it.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32h7xx.cyclo ./Core/Src/system_stm32h7xx.d ./Core/Src/system_stm32h7xx.o ./Core/Src/system_stm32h7xx.su ./Core/Src/tjpgd.cyclo ./Core/Src/tjpgd.d ./Core/Src/tjpgd.o ./Core/Src/tjpgd.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/voice_asr.cyclo ./Core/Src/voice_asr.d ./Core/Src/voice_asr.o ./Core/Src/voice_asr.su

.PHONY: clean-Core-2f-Src

