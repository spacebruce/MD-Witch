#pragma once

#include <genesis.h>

/*
    ATTACK TYPE
*/
typedef enum PLAYER_ATTACK
{
    PLAYER_ATTACK_EMPTY,    //  no action
    PLAYER_ATTACK_SWIPE,    //  2 frame hitbox around staff
    PLAYER_ATTACK_BLAST,    //  Projectile no. 1
} PLAYER_ATTACK;
typedef enum PLAYER_ATTACK_FACING
{
    ATTACK_FACING_LEFT,
    ATTACK_FACING_RIGHT,
    ATTACK_FACING_UP,
    ATTACK_FACING_DOWN,
} PLAYER_ATTACK_FACING;

/*
    Handler struct
*/
#define PlayerAttacksMax (10)
struct PlayerAttackData
{
    u16 Lifespan;
    PLAYER_ATTACK Type;
    PLAYER_ATTACK_FACING Direction;
    Sprite* Graphic;
    struct PlayerAttackData* Next;
};

uint16_t PlayerInitAttacks(uint16_t VRAM);
bool PlayerCreateAttack(s16 x, s16 y, PLAYER_ATTACK_FACING direction, PLAYER_ATTACK type);
void PlayerUpdateAttacks();