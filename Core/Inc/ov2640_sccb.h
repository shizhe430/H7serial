#ifndef __OV2640_SCCB_H
#define __OV2640_SCCB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdint.h>

#define OV2640_SCCB_SCL_GPIO_PORT     GPIOB
#define OV2640_SCCB_SCL_GPIO_PIN      GPIO_PIN_4
#define OV2640_SCCB_SDA_GPIO_PORT     GPIOB
#define OV2640_SCCB_SDA_GPIO_PIN      GPIO_PIN_3

void OV2640_SCCB_Init(void);
HAL_StatusTypeDef OV2640_SCCB_WriteReg(uint8_t dev, uint8_t reg, uint8_t data);
HAL_StatusTypeDef OV2640_SCCB_ReadReg(uint8_t dev, uint8_t reg, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif
