#ifndef IO_CONTROL_H
#define IO_CONTROL_H

#include "traffic_fsm.h"

void IO_Init(void);

void IO_SetLight(Direction dir, LightState state);

void IO_SetPedestrianSignal(Direction dir, PedestrianState state);

uint8_t IO_ReadPedestrianButton(Direction dir);
uint8_t IO_ReadEmergencySensor(void);

void IO_UpdateCountdown(uint8_t seconds);

#endif