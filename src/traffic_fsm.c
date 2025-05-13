#include "../header/config.h"
#include "../header/io_control.h"
#include "../header/timer.h"
#include "../header/traffic_fsm.h"
#include <stdbool.h>

static TrafficState current_state = STATE_NS_GREEN_EW_RED;
static SystemMode current_mode = MODE_NORMAL;
static uint32_t state_timer = 0;
static bool pedestrian_walk_active = false;

void TrafficFSM_Init(void)
{
    IO_SetLight(DIR_NORTH, LIGHT_RED);
    IO_SetLight(DIR_SOUTH, LIGHT_RED);
    IO_SetLight(DIR_EAST, LIGHT_RED);
    IO_SetLight(DIR_WEST, LIGHT_RED);

    IO_SetLight(DIR_NORTH, LIGHT_GREEN);
    IO_SetLight(DIR_SOUTH, LIGHT_GREEN);
    current_state = STATE_NS_GREEN_EW_RED;
    Timer_Start(10000);
}

void TrafficFSM_Update(void)
{
    if (!Timer_IsExpired())
        return;

    switch (current_mode)
    {
    case MODE_NORMAL:
        HandleNormalMode();
        break;
    case MODE_PEDESTRIAN:
        HandlePedestrianMode();
        break;
    case MODE_EMERGENCY:
        HandleEmergencyMode();
        break;
    }
}

static void HandleNormalMode(void)
{
    switch (current_state)
    {
    case STATE_NS_GREEN_EW_RED:
        IO_SetLight(DIR_NORTH, LIGHT_YELLOW);
        IO_SetLight(DIR_SOUTH, LIGHT_YELLOW);
        current_state = STATE_NS_YELLOW_EW_RED;
        Timer_Start(3000);
        break;

    case STATE_NS_YELLOW_EW_RED:
        IO_SetLight(DIR_NORTH, LIGHT_RED);
        IO_SetLight(DIR_SOUTH, LIGHT_RED);
        IO_SetLight(DIR_EAST, LIGHT_GREEN);
        IO_SetLight(DIR_WEST, LIGHT_GREEN);
        current_state = STATE_EW_GREEN_NS_RED;
        Timer_Start(10000);
        break;

    case STATE_EW_GREEN_NS_RED:
        IO_SetLight(DIR_EAST, LIGHT_YELLOW);
        IO_SetLight(DIR_WEST, LIGHT_YELLOW);
        current_state = STATE_EW_YELLOW_NS_RED;
        Timer_Start(3000);
        break;

    case STATE_EW_YELLOW_NS_RED:
        IO_SetLight(DIR_EAST, LIGHT_RED);
        IO_SetLight(DIR_WEST, LIGHT_RED);
        IO_SetLight(DIR_NORTH, LIGHT_GREEN);
        IO_SetLight(DIR_SOUTH, LIGHT_GREEN);
        current_state = STATE_NS_GREEN_EW_RED;
        Timer_Start(10000);
        break;

    default:
        break;
    }
}

static void HandlePedestrianMode(void)
{
    if (!pedestrian_walk_active)
    {
        IO_SetPedestrianSignal(pedestrian_direction, PEDESTRIAN_WALK);
        pedestrian_walk_active = true;
        Timer_Start(5000);
    }
    else
    {
        IO_SetPedestrianSignal(pedestrian_direction, PEDESTRIAN_FLASH_DONT);
        Timer_Start(3000);

        current_mode = MODE_NORMAL;
        pedestrian_walk_active = false;

        if (current_state == STATE_NS_GREEN_EW_RED || current_state == STATE_NS_YELLOW_EW_RED)
        {
            current_state = STATE_EW_GREEN_NS_RED;
            Timer_Start(10000);
        }
        else
        {
            current_state = STATE_NS_GREEN_EW_RED;
            Timer_Start(10000);
        }
    }
}

static void HandleEmergencyMode(void)
{
    current_mode = MODE_NORMAL;

    if (current_state == STATE_NS_GREEN_EW_RED || current_state == STATE_NS_YELLOW_EW_RED)
    {
        current_state = STATE_EW_GREEN_NS_RED;
        Timer_Start(10000);
    }
    else
    {
        current_state = STATE_NS_GREEN_EW_RED;
        Timer_Start(10000);
    }
}

void TrafficFSM_HandlePedestrianRequest(Direction dir)
{
    if (current_mode != MODE_NORMAL)
        return;

    current_mode = MODE_PEDESTRIAN;
    pedestrian_direction = dir;

    IO_SetLight(DIR_NORTH, LIGHT_RED);
    IO_SetLight(DIR_SOUTH, LIGHT_RED);
    IO_SetLight(DIR_EAST, LIGHT_RED);
    IO_SetLight(DIR_WEST, LIGHT_RED);

    current_state = STATE_ALL_RED_PEDESTRIAN;
    pedestrian_walk_active = false;
    Timer_Start(0);
}

void TrafficFSM_HandleEmergency(void)
{
    current_mode = MODE_EMERGENCY;

    IO_SetLight(DIR_NORTH, LIGHT_RED);
    IO_SetLight(DIR_SOUTH, LIGHT_RED);
    IO_SetLight(DIR_EAST, LIGHT_RED);
    IO_SetLight(DIR_WEST, LIGHT_RED);

    current_state = STATE_ALL_RED_EMERGENCY;
    Timer_Start(8000);
}