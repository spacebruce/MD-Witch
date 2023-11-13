#ifndef STATEGAME_H
#define STATEGAME_H

#include <genesis.h>
#include "States/State.h"

void StateGame_Start();
void StateGame_End();
void StateGame_Tick();

extern struct StateType StateGame;

#endif