#pragma once

#include <genesis.h>

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

bool PlayerCreateAttack(s16 x, s16 y, PLAYER_ATTACK_FACING direction, PLAYER_ATTACK type);