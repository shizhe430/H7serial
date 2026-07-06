#ifndef __PUMP_H
#define __PUMP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    PUMP_COMMAND_STOP = 0U,
    PUMP_COMMAND_FAST = 1U,
    PUMP_COMMAND_SLOW = 2U
} pump_command_t;

void Pump_Init(void);
void Pump_SetSpeed(uint16_t duty);
void Pump_Stop(void);
void Pump_Start(void);
void Pump_StartFast(void);
void Pump_StartSlow(void);
void Pump_ApplyCommand(uint8_t command);
uint16_t Pump_GetLastDuty(void);
uint32_t Pump_GetPinLevel(void);
int AI_GetState(void);

#ifdef __cplusplus
}
#endif

#endif /* __PUMP_H */
