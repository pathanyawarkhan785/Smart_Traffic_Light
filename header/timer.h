#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

void Timer_Init(void);

void Timer_Start(uint32_t milliseconds);

bool Timer_IsExpired(void);

uint32_t Timer_GetCurrentValue(void);

#endif