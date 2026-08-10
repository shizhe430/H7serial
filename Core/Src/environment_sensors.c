#include "environment_sensors.h"

#include "delay.h"
#include "camera_light.h"
#include "lcd.h"
#include "main.h"
#include "usart.h"

#include <stdio.h>

#define ENV_DS_PIN                 DS18B20_DQ_Pin
#define ENV_DS_PORT                DS18B20_DQ_GPIO_Port
#define ENV_DS_CONVERSION_MS       750U
#define ENV_DS_RETRY_MS            1000U
#define ENV_SENSOR_LOG_MS          1000U
static uint8_t s_initialized;
static uint8_t s_water_present;
static uint8_t s_ds_present;
static uint8_t s_temperature_valid;
static uint8_t s_conversion_active;
static int16_t s_temperature_tenths = ENVIRONMENT_TEMPERATURE_INVALID;
static uint32_t s_conversion_started_ms;
static uint32_t s_last_ds_attempt_ms;
static uint32_t s_last_log_ms;

static void environment_ds_output(void)
{
    ENV_DS_PORT->OTYPER &= ~ENV_DS_PIN;
    ENV_DS_PORT->MODER = (ENV_DS_PORT->MODER & ~(3UL << (12U * 2U))) |
                         (1UL << (12U * 2U));
}

static void environment_ds_input(void)
{
    ENV_DS_PORT->MODER &= ~(3UL << (12U * 2U));
}

static void environment_ds_release(void)
{
    HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_SET);
    environment_ds_input();
}

static uint8_t environment_ds_reset(void)
{
    uint32_t primask = __get_PRIMASK();
    uint32_t wait_us = 0U;
    uint8_t present = 0U;

    __disable_irq();
    environment_ds_output();
    HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_RESET);
    delay_us(750U);
    HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_SET);
    environment_ds_input();
    delay_us(15U);
    while (wait_us < 200U)
    {
        if (HAL_GPIO_ReadPin(ENV_DS_PORT, ENV_DS_PIN) == GPIO_PIN_RESET)
        {
            present = 1U;
            break;
        }
        delay_us(1U);
        wait_us++;
    }
    wait_us = 0U;
    while ((present != 0U) &&
           (HAL_GPIO_ReadPin(ENV_DS_PORT, ENV_DS_PIN) == GPIO_PIN_RESET) &&
           (wait_us < 240U))
    {
        delay_us(1U);
        wait_us++;
    }
    if (wait_us >= 240U)
    {
        present = 0U;
    }
    environment_ds_release();
    if (primask == 0U)
    {
        __enable_irq();
    }
    return present;
}

static void environment_ds_write_bit(uint8_t value)
{
    uint32_t primask = __get_PRIMASK();

    __disable_irq();
    environment_ds_output();
    HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_RESET);
    if (value != 0U)
    {
        delay_us(6U);
        HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_SET);
        delay_us(64U);
    }
    else
    {
        delay_us(60U);
        HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_SET);
        delay_us(10U);
    }
    environment_ds_release();
    if (primask == 0U)
    {
        __enable_irq();
    }
}

static uint8_t environment_ds_read_bit(void)
{
    uint32_t primask = __get_PRIMASK();
    uint8_t value;

    __disable_irq();
    environment_ds_output();
    HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_RESET);
    delay_us(3U);
    HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_SET);
    environment_ds_input();
    delay_us(12U);
    value = (HAL_GPIO_ReadPin(ENV_DS_PORT, ENV_DS_PIN) == GPIO_PIN_SET) ? 1U : 0U;
    delay_us(51U);
    environment_ds_release();
    if (primask == 0U)
    {
        __enable_irq();
    }
    return value;
}

static void environment_ds_write_byte(uint8_t value)
{
    uint8_t bit;

    for (bit = 0U; bit < 8U; bit++)
    {
        environment_ds_write_bit((uint8_t)(value & 1U));
        value >>= 1U;
    }
}

static uint8_t environment_ds_read_byte(void)
{
    uint8_t bit;
    uint8_t value = 0U;

    for (bit = 0U; bit < 8U; bit++)
    {
        value |= (uint8_t)(environment_ds_read_bit() << bit);
    }
    return value;
}

static uint8_t environment_ds_crc8(const uint8_t *data, uint8_t length)
{
    uint8_t byte_index;
    uint8_t crc = 0U;

    for (byte_index = 0U; byte_index < length; byte_index++)
    {
        uint8_t bit;
        uint8_t value = data[byte_index];
        for (bit = 0U; bit < 8U; bit++)
        {
            uint8_t mix = (uint8_t)((crc ^ value) & 1U);
            crc >>= 1U;
            if (mix != 0U)
            {
                crc ^= 0x8CU;
            }
            value >>= 1U;
        }
    }
    return crc;
}

static uint8_t environment_ds_start_conversion(void)
{
    if (environment_ds_reset() == 0U)
    {
        return 1U;
    }
    environment_ds_write_byte(0xCCU);
    environment_ds_write_byte(0x44U);
    return 0U;
}

static uint8_t environment_ds_read_temperature(int16_t *temperature_tenths)
{
    uint8_t scratchpad[9];
    uint8_t index;
    int16_t raw;
    int32_t tenths;

    if (environment_ds_reset() == 0U)
    {
        return 1U;
    }
    environment_ds_write_byte(0xCCU);
    environment_ds_write_byte(0xBEU);
    for (index = 0U; index < 9U; index++)
    {
        scratchpad[index] = environment_ds_read_byte();
    }
    if (environment_ds_crc8(scratchpad, 8U) != scratchpad[8])
    {
        return 2U;
    }

    raw = (int16_t)(((uint16_t)scratchpad[1] << 8U) | scratchpad[0]);
    tenths = ((int32_t)raw * 10L) / 16L;
    if (tenths > 1250L || tenths < -550L)
    {
        return 3U;
    }
    *temperature_tenths = (int16_t)tenths;
    return 0U;
}

static void environment_sensor_log(uint32_t now_ms)
{
    char message[192];
    int length;

    if ((now_ms - s_last_log_ms) < ENV_SENSOR_LOG_MS)
    {
        return;
    }
    s_last_log_ms = now_ms;
    length = snprintf(message, sizeof(message),
                      "[SENSOR] tank=%s pi1=%u ds18b20=%s dq=%u temp=%s%ld.%01ldC light=%u/999 lcd=0x%04X %ux%u\r\n",
                      (s_water_present != 0U) ? "present" : "empty",
                      (unsigned int)HAL_GPIO_ReadPin(TANK_LEVEL_GPIO_Port, TANK_LEVEL_Pin),
                      (s_ds_present != 0U) ? "present" : "missing",
                      (unsigned int)HAL_GPIO_ReadPin(ENV_DS_PORT, ENV_DS_PIN),
                      (s_temperature_valid != 0U && s_temperature_tenths < 0) ? "-" : "",
                      (long)((s_temperature_valid != 0U) ?
                             ((s_temperature_tenths < 0) ? -s_temperature_tenths : s_temperature_tenths) / 10 : 0),
                      (long)((s_temperature_valid != 0U) ?
                             ((s_temperature_tenths < 0) ? -s_temperature_tenths : s_temperature_tenths) % 10 : 0),
                      (unsigned int)CameraLight_GetDuty(),
                      (unsigned int)lcddev.id,
                      (unsigned int)lcddev.width,
                      (unsigned int)lcddev.height);
    if (length > 0)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)message, (uint16_t)length, 100U);
    }
}

void EnvironmentSensors_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    delay_init();
    ENV_DS_PORT->PUPDR = (ENV_DS_PORT->PUPDR & ~(3UL << (12U * 2U))) |
                         (1UL << (12U * 2U));
    ENV_DS_PORT->OSPEEDR |= (3UL << (12U * 2U));
    environment_ds_output();
    HAL_GPIO_WritePin(ENV_DS_PORT, ENV_DS_PIN, GPIO_PIN_SET);

    s_water_present = (HAL_GPIO_ReadPin(TANK_LEVEL_GPIO_Port, TANK_LEVEL_Pin) == GPIO_PIN_SET) ? 1U : 0U;
    s_ds_present = (environment_ds_reset() != 0U) ? 1U : 0U;
    s_temperature_valid = 0U;
    s_temperature_tenths = ENVIRONMENT_TEMPERATURE_INVALID;
    s_conversion_active = 0U;
    s_conversion_started_ms = 0U;
    s_last_ds_attempt_ms = 0U;
    s_last_log_ms = HAL_GetTick() - ENV_SENSOR_LOG_MS;
    s_initialized = 1U;
}

uint8_t EnvironmentSensors_Poll(uint32_t now_ms)
{
    uint8_t changed = 0U;
    uint8_t water_present;

    if (s_initialized == 0U)
    {
        return 0U;
    }

    water_present = (HAL_GPIO_ReadPin(TANK_LEVEL_GPIO_Port, TANK_LEVEL_Pin) == GPIO_PIN_SET) ? 1U : 0U;
    if (water_present != s_water_present)
    {
        s_water_present = water_present;
        changed = 1U;
    }

    if ((s_conversion_active != 0U) &&
        ((now_ms - s_conversion_started_ms) >= ENV_DS_CONVERSION_MS))
    {
        int16_t temperature_tenths;
        uint8_t status = environment_ds_read_temperature(&temperature_tenths);
        s_conversion_active = 0U;
        if (status == 0U)
        {
            if ((s_temperature_valid == 0U) || (s_temperature_tenths != temperature_tenths))
            {
                changed = 1U;
            }
            s_ds_present = 1U;
            s_temperature_valid = 1U;
            s_temperature_tenths = temperature_tenths;
        }
        else
        {
            s_temperature_valid = 0U;
            s_temperature_tenths = ENVIRONMENT_TEMPERATURE_INVALID;
            s_ds_present = (status == 1U) ? 0U : s_ds_present;
            changed = 1U;
        }
        s_last_ds_attempt_ms = now_ms;
    }

    if ((s_conversion_active == 0U) &&
        ((now_ms - s_last_ds_attempt_ms) >= ENV_DS_RETRY_MS))
    {
        s_last_ds_attempt_ms = now_ms;
        if (environment_ds_start_conversion() == 0U)
        {
            s_ds_present = 1U;
            s_conversion_active = 1U;
            s_conversion_started_ms = now_ms;
        }
        else
        {
            s_ds_present = 0U;
            s_temperature_valid = 0U;
            s_temperature_tenths = ENVIRONMENT_TEMPERATURE_INVALID;
        }
    }

    environment_sensor_log(now_ms);
    return changed;
}

uint8_t EnvironmentSensors_GetWaterPresent(void)
{
    return s_water_present;
}

uint8_t EnvironmentSensors_IsTemperatureValid(void)
{
    return s_temperature_valid;
}

uint8_t EnvironmentSensors_IsDs18b20Present(void)
{
    return s_ds_present;
}

int16_t EnvironmentSensors_GetTemperatureTenths(void)
{
    return s_temperature_tenths;
}
