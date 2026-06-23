#ifndef __OV2640_H
#define __OV2640_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdint.h>

#define OV2640_OK            0U
#define OV2640_ERROR         1U

#define OV2640_SCCB_ADDR     0x60U
#define OV2640_REG_ID_MIDH   0x1CU
#define OV2640_REG_ID_MIDL   0x1DU
#define OV2640_REG_ID_PIDH   0x0AU
#define OV2640_REG_ID_PIDL   0x0BU

uint8_t OV2640_Init(void);
uint8_t OV2640_IsReady(void);
uint8_t OV2640_ProbeID(uint16_t *mid, uint16_t *pid);
uint8_t OV2640_Probe(uint16_t *mid, uint16_t *pid);  /* 仅上电+复位+SCCB+读ID */
uint8_t OV2640_SetOutputFormatJPEG(void);
uint8_t OV2640_SetOutputSize(uint16_t width, uint16_t height);
uint8_t OV2640_StartSnapshot(void);
uint8_t OV2640_StopCapture(void);
void OV2640_AttachFrameBuffer(uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __OV2640_H */
