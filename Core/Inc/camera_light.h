#ifndef __CAMERA_LIGHT_H
#define __CAMERA_LIGHT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CAMERA_LIGHT_PWM_MAX 999U

void CameraLight_Init(void);
void CameraLight_SetDuty(uint16_t duty);
uint16_t CameraLight_GetDuty(void);

#ifdef __cplusplus
}
#endif

#endif /* __CAMERA_LIGHT_H */
