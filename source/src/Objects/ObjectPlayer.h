#pragma once
#include "ObjectBase.h"

#include <genesis.h>

typedef enum 
{
    PlayerStateStanding, PlayerStateWalking, 
} PlayerAnimationState;

typedef struct ObjectPlayer
{
    struct ObjectBase Base;
    uint8_t ButtonFrames[8];
    //
    PlayerAnimationState AnimationState;
    int16_t AnimationTick;
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

void ObjectPlayerUpdate(void* object);
void ObjectPlayerInit(void* object);
void ObjectPlayerFree(void* object);
void ObjectPlayerInput(ObjectPlayer *Player, uint8_t changed);