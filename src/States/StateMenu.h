#ifndef STATEMENU_H
#define STATEMENU_H

#include <genesis.h>
#include "States/State.h"

void StateMenu_Start();
void StateMenu_End();
void StateMenu_Tick();

extern struct StateType StateMenu;

#endif