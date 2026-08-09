#ifndef __ENVIRONMENT_SENSORS_H
#define __ENVIRONMENT_SENSORS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENVIRONMENT_TEMPERATURE_INVALID ((int16_t)-32768)

void EnvironmentSensors_Init(void);
uint8_t EnvironmentSensors_Poll(uint32_t now_ms);
uint8_t EnvironmentSensors_GetWaterPresent(void);
uint8_t EnvironmentSensors_IsTemperatureValid(void);
uint8_t EnvironmentSensors_IsDs18b20Present(void);
int16_t EnvironmentSensors_GetTemperatureTenths(void);

#ifdef __cplusplus
}
#endif

#endif /* __ENVIRONMENT_SENSORS_H */
