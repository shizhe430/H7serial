#include "voice_asr.h"

static HAL_StatusTypeDef asr_i2c_write_reg(uint8_t dev7_addr,
                                           uint8_t reg_addr,
                                           const uint8_t *data,
                                           uint16_t len)
{
    return HAL_I2C_Mem_Write(&hi2c4,
                             (uint16_t)(dev7_addr << 1),
                             (uint16_t)reg_addr,
                             I2C_MEMADD_SIZE_8BIT,
                             (uint8_t *)data,
                             len,
                             ASR_I2C_WRITE_TIMEOUT_MS);
}

static HAL_StatusTypeDef asr_i2c_read_reg(uint8_t dev7_addr,
                                          uint8_t reg_addr,
                                          uint8_t *data,
                                          uint16_t len)
{
    return HAL_I2C_Mem_Read(&hi2c4,
                            (uint16_t)(dev7_addr << 1),
                            (uint16_t)reg_addr,
                            I2C_MEMADD_SIZE_8BIT,
                            data,
                            len,
                            ASR_I2C_READ_TIMEOUT_MS);
}

void WriteOneByte(uint16_t addr, uint8_t data)
{
    uint8_t payload = data;

    (void)addr;
    (void)asr_i2c_write_reg(ASR_DEV_ADDR_7BIT, ASR_REG_ADD_WORD, &payload, 1U);
}

void Asr_Speak(uint8_t cmd, uint8_t idNum)
{
    (void)Asr_SpeakChecked(cmd, idNum);
}

HAL_StatusTypeDef Asr_SpeakChecked(uint8_t cmd, uint8_t idNum)
{
    uint8_t payload[2];
    HAL_StatusTypeDef status;

    if ((cmd != ASR_COMMAND) && (cmd != ASR_ANNOUNCER))
    {
        return HAL_ERROR;
    }

    payload[0] = cmd;
    payload[1] = idNum;
    status = asr_i2c_write_reg(ASR_DEV_ADDR_7BIT, ASR_REG_SPEAK, payload, 2U);
    if (status == HAL_OK)
    {
        HAL_Delay(20U);
    }
    return status;
}

HAL_StatusTypeDef Asr_ResultRead(uint8_t *result)
{
    if (result == NULL)
    {
        return HAL_ERROR;
    }

    return asr_i2c_read_reg(ASR_DEV_ADDR_7BIT, ASR_REG_RESULT, result, 1U);
}

int Asr_Result(void)
{
    uint8_t result = 0U;

    if (Asr_ResultRead(&result) == HAL_OK)
    {
        HAL_Delay(20U);
        return (int)result;
    }

    return 0;
}
