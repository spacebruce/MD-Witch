#pragma once
#include "ObjectBase.h"

#include <genesis.h>

typedef struct ObjectPlayer
{
    struct ObjectBase Base;
    fix32 X;
    fix32 Y;
    // Physics
    fix16 VelocityX;
    fix16 VelocityY;
    bool OnFloor;

    // steering
    uint8_t state;
    uint8_t changed;

} ObjectPlayer;

void ObjectPlayerUpdate(ObjectPlayer *object);
void ObjectPlayerCreate(ObjectPlayer *object);
void ObjectPlayerInput(ObjectPlayer *object, uint8_t state, uint8_t changed);