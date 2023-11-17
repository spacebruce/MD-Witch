#pragma once

#include <genesis.h>
#include "States/State.h"

void StateGame_Start();
void StateGame_End();
void StateGame_Tick();
void StateGame_EndTick();

extern struct StateType StateGame;