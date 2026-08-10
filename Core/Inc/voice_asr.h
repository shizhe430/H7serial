#ifndef __VOICE_ASR_H
#define __VOICE_ASR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "i2c.h"

#define ASR_DEV_ADDR_7BIT        0x34U
#define ASR_REG_RESULT           0x64U
#define ASR_REG_SPEAK            0x6EU
#define ASR_REG_ADD_WORD         0xA0U

#define ASR_COMMAND              0x00U
#define ASR_ANNOUNCER            0xFFU

#define ASR_I2C_WRITE_TIMEOUT_MS 10U
#define ASR_I2C_READ_TIMEOUT_MS  5U

#define ASR_WORD_HALF_WATER      0x01U
#define ASR_WORD_FULL_WATER      0x02U
#define ASR_WORD_HALF_HOT_WATER  0x03U
#define ASR_WORD_FULL_HOT_WATER  0x04U
#define ASR_WORD_TODAY_CONSUMED   0x05U
#define ASR_WORD_TODAY_REMAINING  0x06U
#define ASR_WORD_ROOM_TEMPERATURE 0x07U
#define ASR_WORD_CHILD_LOCK_STATE 0x08U
#define ASR_WORD_CHILD_LOCK_ON    0x09U
#define ASR_WORD_CHILD_LOCK_OFF   0x0AU
#define ASR_WORD_STOP_WATER       0x0BU
#define ASR_WORD_NEXT_INTERVAL    0x0CU
#define ASR_WORD_LAST_VOLUME      0x0DU
#define ASR_WORD_NEXT_VOLUME      0x0EU
#define ASR_WORD_TANK_STATE       0x0FU

#define ASR_SPEAK_DISPENSING     0x10U
#define ASR_SPEAK_ABORTED        0x11U
#define ASR_SPEAK_DONE           0x12U
#define ASR_SPEAK_CUP_DETECTED   0x13U

#define ASR_SPEAK_TODAY_NONE     0x20U
#define ASR_SPEAK_TODAY_100      0x21U
#define ASR_SPEAK_TODAY_OVER     0x35U
#define ASR_SPEAK_TODAY_INVALID  0x36U

#define ASR_SPEAK_REMAINING_DONE    0x40U
#define ASR_SPEAK_REMAINING_100     0x41U
#define ASR_SPEAK_REMAINING_OVER    0x55U
#define ASR_SPEAK_REMAINING_INVALID 0x56U

#define ASR_SPEAK_TEMP_BELOW_10  0x60U
#define ASR_SPEAK_TEMP_10        0x61U
#define ASR_SPEAK_TEMP_20        0x62U
#define ASR_SPEAK_TEMP_25        0x63U
#define ASR_SPEAK_TEMP_30        0x64U
#define ASR_SPEAK_TEMP_40        0x65U
#define ASR_SPEAK_TEMP_OVER_40   0x66U
#define ASR_SPEAK_TEMP_INVALID   0x67U

#define ASR_SPEAK_CHILD_LOCK_ON_STATE  0x70U
#define ASR_SPEAK_CHILD_LOCK_OFF_STATE 0x71U
#define ASR_SPEAK_CHILD_LOCK_ON_DONE   0x72U
#define ASR_SPEAK_CHILD_LOCK_OFF_DONE  0x73U
#define ASR_SPEAK_CHILD_LOCK_BLOCKED   0x74U
#define ASR_SPEAK_STOPPED              0x75U

#define ASR_SPEAK_INTERVAL_NOW      0x80U
#define ASR_SPEAK_INTERVAL_10       0x81U
#define ASR_SPEAK_INTERVAL_INVALID  0x88U

#define ASR_SPEAK_LAST_NONE         0x90U
#define ASR_SPEAK_LAST_100          0x91U
#define ASR_SPEAK_LAST_OVER         0x96U
#define ASR_SPEAK_LAST_INVALID      0x97U

#define ASR_SPEAK_NEXT_NONE         0xA0U
#define ASR_SPEAK_NEXT_100          0xA1U
#define ASR_SPEAK_NEXT_OVER         0xA6U
#define ASR_SPEAK_NEXT_INVALID      0xA7U

#define ASR_SPEAK_TANK_NORMAL       0xB0U
#define ASR_SPEAK_TANK_EMPTY        0xB1U
#define ASR_SPEAK_TANK_UNKNOWN      0xB2U

void WriteOneByte(uint16_t addr, uint8_t data);
void Asr_Speak(uint8_t cmd, uint8_t idNum);
int Asr_Result(void);
HAL_StatusTypeDef Asr_SpeakChecked(uint8_t cmd, uint8_t idNum);
HAL_StatusTypeDef Asr_ResultRead(uint8_t *result);

#ifdef __cplusplus
}
#endif

#endif /* __VOICE_ASR_H */
