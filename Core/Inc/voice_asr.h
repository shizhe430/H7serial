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

#define ASR_I2C_TIMEOUT_MS       100U

#define ASR_WORD_HALF_WATER      0x01U
#define ASR_WORD_FULL_WATER      0x02U
#define ASR_WORD_HALF_HOT_WATER  0x03U
#define ASR_WORD_FULL_HOT_WATER  0x04U

#define ASR_SPEAK_DISPENSING     0x10U
#define ASR_SPEAK_ABORTED        0x11U
#define ASR_SPEAK_DONE           0x12U
#define ASR_SPEAK_CUP_DETECTED   0x13U

void WriteOneByte(uint16_t addr, uint8_t data);
void Asr_Speak(uint8_t cmd, uint8_t idNum);
int Asr_Result(void);

#ifdef __cplusplus
}
#endif

#endif /* __VOICE_ASR_H */
