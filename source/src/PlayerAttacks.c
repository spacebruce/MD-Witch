#include "PlayerAttacks.h"
#include <resources.h>
#include "GameContext.h"

struct PlayerAttackData PlayerAttackList[PlayerAttacksMax];
u16 PlayerAttacksRunning = 0;
u16 PlayerAttackSlot = 0;

u16 AttackSpriteVRAM[4];

// Attack definitions
void AttackSwipeInit(struct PlayerAttackData* Attack)
{
}
void AttackSwipeTick(struct PlayerAttackData* Attack)
{
}
void AttackBlastInit(struct PlayerAttackData* Attack)
{
}
void AttackBlastTick(struct PlayerAttackData* Attack)
{
}

uint16_t PlayerInitAttacks(uint16_t VRAM)
{
    memset(PlayerAttackList, '\0', sizeof(PlayerAttackList));
    PlayerAttacksRunning = 0;
    PlayerAttackSlot = 0;
    // Graphics

    //Animation* anim = sprCursor.animations[0];
    //for(int16_t i = 0; i < anim->numFrame; i++)
    //{
    //    TileSet* tileset = anim->frames[i]->tileset;
    //    VDP_loadTileSet(tileset, VRAM, TRUE);
    //    AttackSpriteVRAM[i] = VRAM;
    //    VRAM += tileset->numTile;
    //}

    //PlayerShootSPR = SPR_addSprite(&sprCursor, 0, 0, TILE_ATTR_FULL(PAL2, TRUE, FALSE, FALSE, 0));
    //SPR_setAutoTileUpload(PlayerShootSPR, false);

    return VRAM;
}
bool PlayerCreateAttack(s16 X, s16 Y, PLAYER_ATTACK_FACING Direction, PLAYER_ATTACK Type)
{
    if(DEBUG_MODE)  kprintf("ATTACK : %i/%i", (int)PlayerAttacksRunning, PlayerAttacksMax);
    
    if (PlayerAttacksRunning == PlayerAttacksMax)
    {
        return false;
    }

    switch(Type)
    {
        case PLAYER_ATTACK_SWIPE:   AttackSwipeInit(&PlayerAttackList[PlayerAttacksRunning]);   break;
        case PLAYER_ATTACK_BLAST:   AttackBlastInit(&PlayerAttackList[PlayerAttacksRunning]);   break;
    }

    PlayerAttacksRunning++;
    
    return false;
}
void PlayerUpdateAttacks()
{
    for (int i = 0; i < PlayerAttacksRunning; i++)
    {
        struct PlayerAttackData *p = &PlayerAttackList[i];
        switch(p->Type)
        {
            case PLAYER_ATTACK_SWIPE:   AttackSwipeTick(&PlayerAttackList[PlayerAttacksRunning]);   break;
            case PLAYER_ATTACK_BLAST:   AttackBlastTick(&PlayerAttackList[PlayerAttacksRunning]);   break;
        }
        p->Lifespan--;
        if (p->Lifespan == 0)
        {
            PlayerAttackList[i] = PlayerAttackList[--PlayerAttacksRunning];
            i--;
        }
    }
}