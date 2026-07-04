#ifndef __CAMERA_APP_H
#define __CAMERA_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#define APP_MODE_XCAM_VIEW  0U
#define APP_MODE_AI_INFER   1U
#define APP_MODE_AI_TEST_IMAGE 2U
#ifndef APP_MODE
#define APP_MODE        APP_MODE_XCAM_VIEW
#endif

void CameraApp_Init(void);
void CameraApp_Run(void);

/* ISR 回调 —— 由 HAL_DCMI_FrameEventCallback 调用 */
void CameraApp_SignalFrameDone(void);

#ifdef __cplusplus
}
#endif

#endif /* __CAMERA_APP_H */
