#pragma once
#include "ObjectBase.h"

#include <genesis.h>

typedef struct ObjectPlayer
{
    struct ObjectBase Base;
    uint8_t ButtonFrames[8];
    // 
    s16 MaxHealth;
    s16 Health;
    // Physics
    fix16 VelocityX;
    fix16 VelocityY;
    bool OnFloor;
    bool OnfloorLast;
    uint8_t CoyoteFrames;
    uint8_t JumpHold;
    // steering
    uint8_t changed;
} ObjectPlayer;

void ObjectPlayerUpdate(ObjectPlayer *object);
void ObjectPlayerCreate(ObjectPlayer *object);
void ObjectPlayerInput(ObjectPlayer *object, uint8_t changed);