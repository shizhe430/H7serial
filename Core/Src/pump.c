#include "camera_app.h"
#include "gpio.h"
#include "pump.h"

#define PUMP_PWM_MAX           999U
#define PUMP_DUTY_FAST         950U
#define PUMP_DUTY_SLOW         450U
#define PUMP_GPIO_TEST_MODE    0U
#define PUMP_FAST_DC_TEST_MODE 1U

static uint8_t s_pump_initialized = 0U;
static uint16_t s_pump_last_duty = 0U;
static uint8_t s_pump_ph6_gpio_mode = 0U;

static void pump_ph6_set_af_mode(void)
{
    GPIOH->MODER &= ~GPIO_MODER_MODE6_Msk;
    GPIOH->MODER |= GPIO_MODER_MODE6_1;
    GPIOH->OTYPER &= ~GPIO_OTYPER_OT6;
    GPIOH->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_Msk;
    GPIOH->PUPDR &= ~GPIO_PUPDR_PUPD6_Msk;
    GPIOH->AFR[0] &= ~GPIO_AFRL_AFSEL6_Msk;
    GPIOH->AFR[0] |= (2UL << GPIO_AFRL_AFSEL6_Pos);
    s_pump_ph6_gpio_mode = 0U;
}

static void pump_ph6_set_gpio_mode(void)
{
    GPIOH->MODER &= ~GPIO_MODER_MODE6_Msk;
    GPIOH->MODER |= GPIO_MODER_MODE6_0;
    GPIOH->OTYPER &= ~GPIO_OTYPER_OT6;
    GPIOH->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_Msk;
    GPIOH->PUPDR &= ~GPIO_PUPDR_PUPD6_Msk;
    s_pump_ph6_gpio_mode = 1U;
}

void Pump_Init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    if (s_pump_initialized != 0U)
    {
        return;
    }

    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOHEN;
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOFEN;

    gpio_init.Pin = GPIO_PIN_7;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOF, &gpio_init);

#if (PUMP_GPIO_TEST_MODE == 0U)
    RCC->APB1LENR |= RCC_APB1LENR_TIM12EN;

    pump_ph6_set_af_mode();

    TIM12->CR1 = 0U;
    TIM12->PSC = 3U;
    TIM12->ARR = PUMP_PWM_MAX;
    TIM12->CCMR1 = 0U;
    TIM12->CCMR1 |= (6UL << TIM_CCMR1_OC1M_Pos);
    TIM12->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM12->CCR1 = 0U;
    TIM12->CCER = TIM_CCER_CC1E;
    TIM12->EGR = TIM_EGR_UG;
    TIM12->CR1 |= TIM_CR1_ARPE;
    TIM12->CR1 |= TIM_CR1_CEN;
#else
    pump_ph6_set_gpio_mode();
    GPIOH->BSRR = ((uint32_t)GPIO_PIN_6 << 16U);
#endif

    s_pump_initialized = 1U;
}

void Pump_SetSpeed(uint16_t duty)
{
#if (PUMP_GPIO_TEST_MODE == 0U)
#if (PUMP_FAST_DC_TEST_MODE != 0U)
    if (duty >= PUMP_DUTY_FAST)
    {
        if (s_pump_ph6_gpio_mode == 0U)
        {
            pump_ph6_set_gpio_mode();
        }

        GPIOH->BSRR = GPIO_PIN_6;
        s_pump_last_duty = PUMP_PWM_MAX;
        return;
    }

    if (s_pump_ph6_gpio_mode != 0U)
    {
        if (duty == 0U)
        {
            GPIOH->BSRR = ((uint32_t)GPIO_PIN_6 << 16U);
            s_pump_last_duty = 0U;
            return;
        }

        pump_ph6_set_af_mode();
    }
#endif

    if (duty > PUMP_PWM_MAX)
    {
        duty = PUMP_PWM_MAX;
    }

    TIM12->CCR1 = duty;
#else
    if (duty == 0U)
    {
        GPIOH->BSRR = ((uint32_t)GPIO_PIN_6 << 16U);
    }
    else
    {
        GPIOH->BSRR = GPIO_PIN_6;
    }
#endif

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
    Pump_SetSpeed(PUMP_DUTY_FAST);
}

void Pump_StartSlow(void)
{
    Pump_Start();
    Pump_SetSpeed(PUMP_DUTY_SLOW);
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
    return (GPIOH->IDR & GPIO_PIN_6) ? 1U : 0U;
}
