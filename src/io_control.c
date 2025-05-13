#include "io_control.h"
// #include "stm32f4xx.h"

void IO_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

    GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0;

    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1);
    EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1;
    EXTI->RTSR |= EXTI_RTSR_TR0 | EXTI_RTSR_TR1;
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
}

void IO_SetLight(Direction dir, LightState state)
{
    switch (dir)
    {
    case DIR_NORTH:
        if (state == LIGHT_RED)
        {
            GPIOA->BSRR = GPIO_BSRR_BS0;
        }
        else if (state == LIGHT_YELLOW)
        {
            GPIOA->BSRR = GPIO_BSRR_BS1;
        }
        else if (state == LIGHT_GREEN)
        {
            GPIOA->BSRR = GPIO_BSRR_BS2;
        }
        break;
    }
}

void IO_SetPedestrianSignal(Direction dir, PedestrianState state)
{
    // control pedestrian signal from here needs to work on this
}