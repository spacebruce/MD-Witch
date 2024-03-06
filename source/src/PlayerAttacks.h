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
    Vect2D_f32 Position;
    Vect2D_f16 Vector;
    s16 Lifespan;
    Sprite* Graphic;
    struct PlayerAttackData* Next;
    PLAYER_ATTACK Type;
    PLAYER_ATTACK_FACING Direction;
};

uint16_t PlayerInitAttacks(uint16_t VRAM);
bool PlayerCreateAttack(fix32 x, fix32 y, PLAYER_ATTACK_FACING direction, PLAYER_ATTACK type);
void PlayerUpdateAttacks(const fix32 CameraX, const fix32 CameraY);