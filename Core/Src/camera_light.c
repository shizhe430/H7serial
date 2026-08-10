#include "camera_light.h"

#include "main.h"

#define CAMERA_LIGHT_DEFAULT_DUTY 400U

static uint16_t s_camera_light_duty;
static uint8_t s_camera_light_initialized;

void CameraLight_Init(void)
{
    if (s_camera_light_initialized != 0U)
    {
        return;
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* PA1 = TIM2_CH2, AF1. TIM2 is independent of the TIM5 pump PWM. */
    CAMERA_LIGHT_GPIO_Port->MODER &= ~GPIO_MODER_MODE1_Msk;
    CAMERA_LIGHT_GPIO_Port->MODER |= GPIO_MODER_MODE1_1;
    CAMERA_LIGHT_GPIO_Port->OTYPER &= ~GPIO_OTYPER_OT1;
    CAMERA_LIGHT_GPIO_Port->OSPEEDR |= GPIO_OSPEEDR_OSPEED1_Msk;
    CAMERA_LIGHT_GPIO_Port->PUPDR &= ~GPIO_PUPDR_PUPD1_Msk;
    CAMERA_LIGHT_GPIO_Port->AFR[0] &= ~GPIO_AFRL_AFSEL1_Msk;
    CAMERA_LIGHT_GPIO_Port->AFR[0] |= (1UL << GPIO_AFRL_AFSEL1_Pos);

    TIM2->CR1 = 0U;
    TIM2->PSC = 9U;
    TIM2->ARR = CAMERA_LIGHT_PWM_MAX;
    TIM2->CCMR1 = 0U;
    TIM2->CCMR1 |= (6UL << TIM_CCMR1_OC2M_Pos);
    TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
    TIM2->CCR2 = CAMERA_LIGHT_DEFAULT_DUTY;
    TIM2->CCER = TIM_CCER_CC2E;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;

    s_camera_light_duty = CAMERA_LIGHT_DEFAULT_DUTY;
    s_camera_light_initialized = 1U;
}

void CameraLight_SetDuty(uint16_t duty)
{
    if (duty > CAMERA_LIGHT_PWM_MAX)
    {
        duty = CAMERA_LIGHT_PWM_MAX;
    }

    TIM2->CCR2 = duty;
    s_camera_light_duty = duty;
}

uint16_t CameraLight_GetDuty(void)
{
    return s_camera_light_duty;
}
