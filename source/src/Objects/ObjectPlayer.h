#pragma once
#include "ObjectBase.h"

#include <genesis.h>

typedef enum 
{
    AnimationStanding, AnimationWalking, 
} PlayerAnimationState;

typedef enum 
{
    PlayerStanding, PlayerWalking,
    PlayerCrouching, PlayerCrawling, PlayerSkidding, 
    PlayerHitting, PlayerShooting, PlayerFishing, 
    PlayerJumping, PlayerFalling, PlayerLanding, PlayerBonked, 
} PlayerControlState;


struct PlayerController
{
    uint8_t Changed;
    uint8_t ButtonFrames[8];
    bool Pressed_A : 1, Pressed_B : 1, Pressed_C : 1, Pressed_Left : 1, Pressed_Right : 1, Pressed_Up : 1, Pressed_Down : 1;
    bool Release_A : 1, Release_B : 1, Release_C : 1, Release_Left : 1, Release_Right : 1, Release_Up : 1, Release_Down : 1;
};
//const size_t CS = sizeof(struct PlayerController);

typedef struct ObjectPlayer
{
    struct ObjectBase Base;
    //
    struct PlayerController Controller;
    PlayerAnimationState AnimationState;
    PlayerControlState State;
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