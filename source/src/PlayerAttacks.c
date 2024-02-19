#include "PlayerAttacks.h"

#include "GameContext.h"

struct PlayerAttackData
{
    PLAYER_ATTACK Type;
    PLAYER_ATTACK_FACING Direction;
    struct PlayerAttackData* Next;
};

#define PlayerAttacksMax (10)
struct PlayerAttackData PlayerAttackList[PlayerAttacksMax];
u16 PlayerAttacksRunning = 0;
u16 PlayerAttackSlot = 0;

bool PlayerCreateAttack(s16 X, s16 Y, PLAYER_ATTACK_FACING Direction, PLAYER_ATTACK Type)
{
    
}

void PlayerUpdateAttacks()
{
    
}