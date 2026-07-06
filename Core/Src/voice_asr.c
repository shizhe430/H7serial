#include "voice_asr.h"

static HAL_StatusTypeDef asr_i2c_write_reg(uint8_t dev7_addr,
                                           uint8_t reg_addr,
                                           const uint8_t *data,
                                           uint16_t len)
{
    return HAL_I2C_Mem_Write(&hi2c2,
                             (uint16_t)(dev7_addr << 1),
                             (uint16_t)reg_addr,
                             I2C_MEMADD_SIZE_8BIT,
                             (uint8_t *)data,
                             len,
                             ASR_I2C_TIMEOUT_MS);
}

static HAL_StatusTypeDef asr_i2c_read_reg(uint8_t dev7_addr,
                                          uint8_t reg_addr,
                                          uint8_t *data,
                                          uint16_t len)
{
    return HAL_I2C_Mem_Read(&hi2c2,
                            (uint16_t)(dev7_addr << 1),
                            (uint16_t)reg_addr,
                            I2C_MEMADD_SIZE_8BIT,
                            data,
                            len,
                            ASR_I2C_TIMEOUT_MS);
}

void WriteOneByte(uint16_t addr, uint8_t data)
{
    uint8_t payload = data;

    (void)addr;
    (void)asr_i2c_write_reg(ASR_DEV_ADDR_7BIT, ASR_REG_ADD_WORD, &payload, 1U);
}

void Asr_Speak(uint8_t cmd, uint8_t idNum)
{
    uint8_t payload[2];

    if ((cmd != ASR_COMMAND) && (cmd != ASR_ANNOUNCER))
    {
        return;
    }

    payload[0] = cmd;
    payload[1] = idNum;
    (void)asr_i2c_write_reg(ASR_DEV_ADDR_7BIT, ASR_REG_SPEAK, payload, 2U);
    HAL_Delay(20U);
}

int Asr_Result(void)
{
    uint8_t result = 0U;

    if (asr_i2c_read_reg(ASR_DEV_ADDR_7BIT, ASR_REG_RESULT, &result, 1U) == HAL_OK)
    {
        HAL_Delay(20U);
        return (int)result;
    }

    return 0;
}
