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
#define APP_MODE_DUAL_CAMERA_DIAG 7U
#define APP_MODE_FACE_DIAG  8U
#define APP_MODE_FACE_AI_DIAG 9U
#define APP_MODE_QSPI_PROGRAM 10U
#define APP_MODE_FACE_AI_VISUAL 11U
#define APP_MODE_OPENMV_HOST_DIAG 12U
#define APP_MODE_OPENMV_BOARD_DIAG 13U
#define APP_MODE_WATER_ROI_CALIB 14U
#ifndef APP_MODE
#define APP_MODE  APP_MODE_AI_VISUAL
#endif

/* Calibrated dataset view: SVGA sensor window, 320x240 JPEG and shifted ROI. */
#ifndef WATER_AI_ROI_OFFSET_X
#define WATER_AI_ROI_OFFSET_X 50
#endif
#ifndef WATER_AI_ROI_OFFSET_Y
#define WATER_AI_ROI_OFFSET_Y (-1)
#endif
#ifndef WATER_AI_VIEW_FILL
#define WATER_AI_VIEW_FILL 180U
#endif

#if ((WATER_AI_ROI_OFFSET_X < -128) || (WATER_AI_ROI_OFFSET_X > 127) || \
     (WATER_AI_ROI_OFFSET_Y < -128) || (WATER_AI_ROI_OFFSET_Y > 127))
#error "WATER_AI_ROI_OFFSET_X/Y must fit the signed AIV1 metadata range"
#endif

/* Dataset-only PA1 lighting cycle. Production builds must leave this disabled. */
#ifndef WATER_DATASET_LIGHT_CYCLE_ENABLE
#define WATER_DATASET_LIGHT_CYCLE_ENABLE 0U
#endif
#ifndef WATER_DATASET_LIGHT_PRIMARY_DUTY
#define WATER_DATASET_LIGHT_PRIMARY_DUTY 320U
#endif
#ifndef WATER_DATASET_LIGHT_ALT1_DUTY
#define WATER_DATASET_LIGHT_ALT1_DUTY 180U
#endif
#ifndef WATER_DATASET_LIGHT_ALT2_DUTY
#define WATER_DATASET_LIGHT_ALT2_DUTY 400U
#endif
#ifndef WATER_DATASET_LIGHT_PRIMARY_MS
#define WATER_DATASET_LIGHT_PRIMARY_MS 7000U
#endif
#ifndef WATER_DATASET_LIGHT_ALT_MS
#define WATER_DATASET_LIGHT_ALT_MS 3000U
#endif

#if ((WATER_DATASET_LIGHT_CYCLE_ENABLE != 0U) && (APP_MODE != APP_MODE_AI_VISUAL))
#error "WATER_DATASET_LIGHT_CYCLE_ENABLE is restricted to APP_MODE_AI_VISUAL"
#endif
#if ((WATER_DATASET_LIGHT_PRIMARY_DUTY > 999U) || \
     (WATER_DATASET_LIGHT_ALT1_DUTY > 999U) || \
     (WATER_DATASET_LIGHT_ALT2_DUTY > 999U))
#error "Water dataset light duty must fit the PA1 PWM range 0..999"
#endif
#if (((WATER_DATASET_LIGHT_PRIMARY_DUTY % 4U) != 0U) || \
     ((WATER_DATASET_LIGHT_ALT1_DUTY % 4U) != 0U) || \
     ((WATER_DATASET_LIGHT_ALT2_DUTY % 4U) != 0U))
#error "Water dataset light duty must be divisible by 4 for AIV1 metadata"
#endif
#if ((WATER_DATASET_LIGHT_PRIMARY_MS == 0U) || (WATER_DATASET_LIGHT_ALT_MS == 0U))
#error "Water dataset light phase duration must be nonzero"
#endif

#ifndef CAMERA_APP_ACTIVE_CAMERA
#if ((APP_MODE == APP_MODE_FACE_DIAG) || (APP_MODE == APP_MODE_FACE_AI_DIAG) || \
     (APP_MODE == APP_MODE_FACE_AI_VISUAL) || (APP_MODE == APP_MODE_OPENMV_HOST_DIAG) || \
     (APP_MODE == APP_MODE_OPENMV_BOARD_DIAG))
#define CAMERA_APP_ACTIVE_CAMERA OV2640_CAMERA_FACE
#else
#define CAMERA_APP_ACTIVE_CAMERA OV2640_CAMERA_WATER
#endif
#endif

#ifndef CAMERA_FACE_IDENTITY_ENABLE
#define CAMERA_FACE_IDENTITY_ENABLE 1U
#endif

#ifndef CAMERA_FACE_BACKEND_OPENMV
#define CAMERA_FACE_BACKEND_OPENMV 1U
#endif

/* AS608 driver files are kept as backup, but excluded from the active product flow. */
#ifndef CAMERA_FINGERPRINT_ENABLE
#define CAMERA_FINGERPRINT_ENABLE 0U
#endif

/* Child lock is profile-based: mark a registered face ID, not an estimated age. */
#ifndef CAMERA_FACE_CHILD_LOCK_ENABLE
#define CAMERA_FACE_CHILD_LOCK_ENABLE 0U
#endif

#ifndef CAMERA_FACE_CHILD_USER_ID
#define CAMERA_FACE_CHILD_USER_ID 0U
#endif

/* Camera-independent fallback used only by the timed demonstration branch. */
#ifndef PUMP_TIMED_DEMO_MODE
#define PUMP_TIMED_DEMO_MODE 0U
#endif

/* Temporary pump-only hardware test. Keep 0U for normal operation. */
#ifndef PUMP_FORCE_ON_TEST_MODE
#define PUMP_FORCE_ON_TEST_MODE 0U
#endif

#if ((APP_MODE == APP_MODE_XCAM_VIEW) || (APP_MODE == APP_MODE_COLORBAR_VIEW) || \
     (APP_MODE == APP_MODE_AI_VISUAL) || (APP_MODE == APP_MODE_FACE_AI_VISUAL) || \
     (APP_MODE == APP_MODE_OPENMV_HOST_DIAG) || (APP_MODE == APP_MODE_OPENMV_BOARD_DIAG) || \
     (APP_MODE == APP_MODE_WATER_ROI_CALIB))
#define APP_MODE_STREAM_SILENT 1U
#else
#define APP_MODE_STREAM_SILENT 0U
#endif

void CameraApp_Init(void);
void CameraApp_Run(void);
void CameraApp_LcdHotPressed(void);
void CameraApp_LcdColdPressed(void);

/* ISR 回调 —— 由 HAL_DCMI_FrameEventCallback 调用 */
void CameraApp_SignalFrameDone(void);

#ifdef __cplusplus
}
#endif

#endif /* __CAMERA_APP_H */
