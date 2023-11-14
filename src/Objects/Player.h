#pragma once

#include "Actor.h"

typedef struct
{
    Actor Base;    
    fix16 MomentumX;
    fix16 MomentumY;
} PlayerType;

void PlayerInit(PlayerType* P);
void PlayerFree(PlayerType* P);
void PlayerUpdate(PlayerType* P);
void PlayerDraw(PlayerType* P);
void PlayerInput(PlayerType* P, u16 Joy, u16 Changed, u16 State);