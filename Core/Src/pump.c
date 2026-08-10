#include "camera_app.h"
#include "gpio.h"
#include "pump.h"

#define PUMP_DUTY_FAST_DEFAULT 600U
#define PUMP_DUTY_SLOW         450U
#define PUMP_GPIO_TEST_MODE    0U

static uint8_t s_pump_initialized = 0U;
static uint16_t s_pump_last_duty = 0U;
static uint16_t s_pump_fast_duty = PUMP_DUTY_FAST_DEFAULT;

void Pump_Init(void)
{
    if (s_pump_initialized != 0U)
    {
        return;
    }

    __HAL_RCC_GPIOI_CLK_ENABLE();
    RCC->APB1LENR |= RCC_APB1LENR_TIM5EN;

    GPIOI->MODER &= ~GPIO_MODER_MODE0_Msk;
    GPIOI->MODER |= GPIO_MODER_MODE0_1;
    GPIOI->OTYPER &= ~GPIO_OTYPER_OT0;
    GPIOI->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_Msk;
    GPIOI->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
    GPIOI->AFR[0] &= ~GPIO_AFRL_AFSEL0_Msk;
    GPIOI->AFR[0] |= (2UL << GPIO_AFRL_AFSEL0_Pos);

    TIM5->CR1 = 0U;
    TIM5->PSC = 11U;
    TIM5->ARR = PUMP_PWM_MAX;
    TIM5->CCMR2 = 0U;
    TIM5->CCMR2 |= (6UL << TIM_CCMR2_OC4M_Pos);
    TIM5->CCMR2 |= TIM_CCMR2_OC4PE;
    TIM5->CCR4 = 0U;
    TIM5->CCER = TIM_CCER_CC4E;
    TIM5->EGR = TIM_EGR_UG;
    TIM5->CR1 |= TIM_CR1_ARPE;
    TIM5->CR1 |= TIM_CR1_CEN;

    s_pump_initialized = 1U;
}

void Pump_SetSpeed(uint16_t duty)
{
    if (duty > PUMP_PWM_MAX)
    {
        duty = PUMP_PWM_MAX;
    }

    TIM5->CCR4 = duty;

    s_pump_last_duty = duty;
}

void Pump_Stop(void)
{
    Pump_SetSpeed(0U);
}

void Pump_Start(void)
{
}

void Pump_StartFast(void)
{
    Pump_Start();
    Pump_SetSpeed(s_pump_fast_duty);
}

void Pump_StartSlow(void)
{
    Pump_Start();
    Pump_SetSpeed(PUMP_DUTY_SLOW);
}

void Pump_SetFastDuty(uint16_t duty)
{
    if (duty > PUMP_PWM_MAX)
    {
        duty = PUMP_PWM_MAX;
    }
    s_pump_fast_duty = duty;
}

uint16_t Pump_GetFastDuty(void)
{
    return s_pump_fast_duty;
}

void Pump_ApplyCommand(uint8_t command)
{
    switch (command)
    {
        case PUMP_COMMAND_FAST:
            Pump_StartFast();
            break;

        case PUMP_COMMAND_SLOW:
            Pump_StartSlow();
            break;

        case PUMP_COMMAND_STOP:
        default:
            Pump_Stop();
            break;
    }
}

uint16_t Pump_GetLastDuty(void)
{
    return s_pump_last_duty;
}

uint32_t Pump_GetPinLevel(void)
{
    return (TIM5->CCR4 != 0U) ? 1U : 0U;
}
