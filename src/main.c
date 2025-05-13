#include "../header/traffic_fsm.h"
#include "../header/io_control.h"
#include "../header/config.h"
#include "../header/timer.h"
// #include "stm32f4xx.h" // Device header for STM32F4

volatile uint8_t emergency_flag = 0;
volatile uint8_t pedestrian_request = 0;
Direction pedestrian_direction = DIR_NORTH;

int main(void)
{
    IO_Init();
    Timer_Init();
    TrafficFSM_Init();

    __enable_irq();

    while (1)
    {
        if (emergency_flag)
        {
            TrafficFSM_HandleEmergency();
            emergency_flag = 0;
        }

        if (pedestrian_request)
        {
            TrafficFSM_HandlePedestrianRequest(pedestrian_direction);
            pedestrian_request = 0;
        }

        TrafficFSM_Update();
    }
}

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR0)
    {
        emergency_flag = 1;
        EXTI->PR |= EXTI_PR_PR0;
    }
}

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR1)
    {
        pedestrian_request = 1;
        pedestrian_direction = DIR_NORTH;
        EXTI->PR |= EXTI_PR_PR1;
    }
}