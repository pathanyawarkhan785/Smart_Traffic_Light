#ifndef TRAFFIC_FSM_H
#define TRAFFIC_FSM_H

#include <stdint.h>

typedef enum
{
    DIR_NORTH,
    DIR_SOUTH,
    DIR_EAST,
    DIR_WEST
} Direction;

typedef enum
{
    LIGHT_RED,
    LIGHT_YELLOW,
    LIGHT_GREEN,
    LIGHT_OFF
} LightState;

typedef enum
{
    PEDESTRIAN_OFF,
    PEDESTRIAN_WALK,
    PEDESTRIAN_DONT_WALK,
    PEDESTRIAN_FLASH_DONT
} PedestrianState;

typedef enum
{
    MODE_NORMAL,
    MODE_PEDESTRIAN,
    MODE_EMERGENCY
} SystemMode;

typedef enum
{
    STATE_NS_GREEN_EW_RED,
    STATE_NS_YELLOW_EW_RED,
    STATE_EW_GREEN_NS_RED,
    STATE_EW_YELLOW_NS_RED,
    STATE_ALL_RED_PEDESTRIAN,
    STATE_ALL_RED_EMERGENCY
} TrafficState;

void TrafficFSM_Init(void);
void TrafficFSM_Update(void);
void TrafficFSM_HandlePedestrianRequest(Direction dir);
void TrafficFSM_HandleEmergency(void);
void TrafficFSM_HandleTimerExpiry(void);

extern volatile uint8_t emergency_flag;
extern volatile uint8_t pedestrian_request;

#endif