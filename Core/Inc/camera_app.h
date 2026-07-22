#ifndef __CAMERA_APP_H
#define __CAMERA_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#define APP_MODE_XCAM_VIEW  0U
#define APP_MODE_AI_INFER   1U
#define APP_MODE_AI_TEST_IMAGE 2U
#define APP_MODE_PUMP_CTRL  3U
#define APP_MODE_JPEG_DIAG  4U
#define APP_MODE_COLORBAR_VIEW 5U
#define APP_MODE_AI_VISUAL  6U
#ifndef APP_MODE
#define APP_MODE  APP_MODE_PUMP_CTRL
#endif

/* Camera-independent fallback used only by the timed demonstration branch. */
#ifndef PUMP_TIMED_DEMO_MODE
#define PUMP_TIMED_DEMO_MODE 0U
#endif

/* Temporary pump-only hardware test. Keep 0U for normal operation. */
#ifndef PUMP_FORCE_ON_TEST_MODE
#define PUMP_FORCE_ON_TEST_MODE 0U
#endif

#if ((APP_MODE == APP_MODE_XCAM_VIEW) || (APP_MODE == APP_MODE_COLORBAR_VIEW) || (APP_MODE == APP_MODE_AI_VISUAL))
#define APP_MODE_STREAM_SILENT 1U
#else
#define APP_MODE_STREAM_SILENT 0U
#endif

void CameraApp_Init(void);
void CameraApp_Run(void);

/* ISR 回调 —— 由 HAL_DCMI_FrameEventCallback 调用 */
void CameraApp_SignalFrameDone(void);

#ifdef __cplusplus
}
#endif

#endif /* __CAMERA_APP_H */
