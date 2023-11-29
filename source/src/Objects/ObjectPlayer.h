#pragma once
#include "ObjectBase.h"

#include <genesis.h>

typedef struct ObjectPlayer
{
    struct ObjectBase Base;
    // Physics
    fix16 VelocityX;
    fix16 VelocityY;
    bool OnFloor;

    // steering
    uint8_t state;
    uint8_t changed;

} ObjectPlayer;

void updateObjectPlayer(ObjectPlayer *object);
void createObjectPlayer(ObjectPlayer *object);
void inputObjectPlayer(ObjectPlayer *object, uint8_t state, uint8_t changed);