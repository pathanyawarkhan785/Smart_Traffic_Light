#include "../header/timer.h"
// #include "stm32f4xx.h"

static volatile uint32_t timer_count = 0;
static volatile uint32_t timer_target = 0;
static volatile bool timer_expired = false;

void Timer_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 16000 - 1;
    TIM2->ARR = 0xFFFFFFFF;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM2_IRQn);
}

void Timer_Start(uint32_t milliseconds)
{
    timer_count = 0;
    timer_target = milliseconds;
    timer_expired = false;
}

bool Timer_IsExpired(void)
{
    if (timer_expired)
    {
        timer_expired = false;
        return true;
    }
    return false;
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;
        timer_count++;

        if (timer_count >= timer_target)
        {
            timer_expired = true;
        }
    }
}