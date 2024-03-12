#pragma once
#include "ObjectBase.h"

#include <genesis.h>

#define PlayerAnimStand (0)
#define PlayerAnimTurn (0)
#define PlayerAnimWalk (1)
#define PlayerAnimShoot (2)
#define PlayerAnimHitting (2)
#define PlayerAnimKnockback (3)

typedef int PlayerAnimationState;

typedef enum 
{
    PlayerStanding = 0, PlayerWalking = 1, 
    PlayerCrouching = 2, PlayerCrawling = 3, PlayerSkidding = 4, 
    PlayerHitting = 5, PlayerShooting = 6, PlayerFishing = 6, 
    PlayerJumping = 7, PlayerFalling = 8, PlayerLanding = 9, PlayerBonked = 10, 
    PlayerKnockback = 11,
} PlayerControlState;

struct PlayerController
{
    uint8_t Changed;
    uint8_t ButtonFrames[8];
    uint8_t Pressed_Jump : 1, Pressed_Shoot : 1, Pressed_C : 1, Pressed_Left : 1, Pressed_Right : 1, Pressed_Up : 1, Pressed_Down : 1;
    uint8_t Release_Jump : 1, Release_Shoot : 1, Release_C : 1, Release_Left : 1, Release_Right : 1, Release_Up : 1, Release_Down : 1;
    uint8_t Moving : 1, WalkDir : 1;
};
//static const size_t CS = sizeof(struct PlayerController);

typedef struct ObjectPlayer
{
    struct ObjectBase Base;
    //
    struct PlayerController Controller;
    PlayerAnimationState LastAnimationState, AnimationState;
    PlayerControlState LastState, State;
    uint16_t StateFrame;
    int16_t AnimationTick;
    // 
    s16 MaxHealth;
    s16 Health;
    // Physics
    fix32 x, y;
    fix16 VelocityX;
    fix16 VelocityY;
    bool OnFloor;
    bool OnfloorLast;
    bool CoyoteMode;
    uint8_t CoyoteFrames;
    uint8_t JumpHold;
    // steering
    uint8_t changed;
} ObjectPlayer;

void ObjectPlayerUpdate(void* object);
void ObjectPlayerInit(void* object);
void ObjectPlayerFree(void* object);
void ObjectPlayerInput(ObjectPlayer *Player, uint8_t changed);

//

void HurtPlayer(ObjectPlayer* Player);