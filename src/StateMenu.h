#pragma once
#include <genesis.h>
#include "Types/State.h"

void StateMenu_Start();
void StateMenu_End();
void StateMenu_Tick();

struct StateType StateMenu = 
{
    StateMenu_Start, StateMenu_End, StateMenu_Tick,
};