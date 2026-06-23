#ifndef PUMP_CTRL_H
#define PUMP_CTRL_H

#include "ai_app.h"
#include "main.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    PUMP_STATE_OFF = 0,
    PUMP_STATE_ON,
    PUMP_STATE_FAULT
} pump_state_t;

typedef enum
{
    PUMP_TARGET_EMPTY = 0,
    PUMP_TARGET_QUARTER,
    PUMP_TARGET_HALF,
    PUMP_TARGET_FULL
} pump_target_level_t;

void pump_ctrl_init(void);
void pump_ctrl_set_target(pump_target_level_t target);
pump_state_t pump_ctrl_update(ai_class_t class_id, float confidence);
pump_state_t pump_ctrl_get_state(void);
const char *pump_ctrl_state_name(pump_state_t state);

#ifdef __cplusplus
}
#endif

#endif
