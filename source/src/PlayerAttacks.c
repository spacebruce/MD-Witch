#include "PlayerAttacks.h"
#include <resources.h>
#include "GameContext.h"

struct PlayerAttackData PlayerAttackList[PlayerAttacksMax];
u16 PlayerAttacksRunning = 0;

u16 AttackSpriteVRAM[4];

// Attack definitions
void AttackSwipeInit(struct PlayerAttackData* Attack)
{
    // PLACEHOLDER
    Attack->Lifespan = 10;
}
void AttackSwipeTick(struct PlayerAttackData* Attack)
{
    // PLACEHOLDER
}
void AttackBlastInit(struct PlayerAttackData* Attack)
{
    Sprite* spr = Attack->Graphic;
    SPR_setVRAMTileIndex(spr, AttackSpriteVRAM[0]);
    SPR_setFrame(spr, 0);
    SPR_setVisibility(spr, VISIBLE);
    Attack->Lifespan = 10;
}
void AttackBlastTick(struct PlayerAttackData* Attack)
{
    // PLACEHOLDER
}

uint16_t PlayerInitAttacks(uint16_t VRAM)
{
    memset(PlayerAttackList, '\0', sizeof(PlayerAttackList));
    PlayerAttacksRunning = 0;
    // Graphics

    /*
        Allocate every sprite... Maybe fix later to recycle vram 
    */
    Animation* anim = sprCursor.animations[0];
    for(int16_t i = 0; i < anim->numFrame; i++)
    {
        TileSet* tileset = anim->frames[i]->tileset;
        VDP_loadTileSet(tileset, VRAM, TRUE);
        AttackSpriteVRAM[i] = VRAM;
        VRAM += tileset->numTile;
    }

    /*
        Give every possible attack instance a sprite, but switch uploading OFF so they can share
        graphics resources
    */
    for(int16_t i = 0; i < PlayerAttacksMax; ++i)
    {
        struct PlayerAttackData* A = &PlayerAttackList[i];
        A->Graphic = SPR_addSprite(&sprCursor, 64,64, TILE_ATTR_FULL(PAL2, TRUE, FALSE, FALSE, 0));
        SPR_setAutoTileUpload(A->Graphic, false);
        SPR_setVisibility(A->Graphic, HIDDEN);
    }

    return VRAM;
}
bool PlayerCreateAttack(fix32 X, fix32 Y, PLAYER_ATTACK_FACING Direction, PLAYER_ATTACK Type)
{
    #if(DEBUG_MODE)  
        kprintf("ATTACK : %i/%i", (int)PlayerAttacksRunning, PlayerAttacksMax);
    #endif
    
    if (PlayerAttacksRunning == PlayerAttacksMax)
    {
        return false;
    }

    struct PlayerAttackData* Attack = &PlayerAttackList[PlayerAttacksRunning];

    Attack->Position.x = X;
    Attack->Position.y = Y;

    switch(Type)
    {
        case PLAYER_ATTACK_SWIPE:   AttackSwipeInit(Attack);   break;
        case PLAYER_ATTACK_BLAST:   AttackBlastInit(Attack);   break;
        default:
            return false;
        break;
    }

    PlayerAttacksRunning++;
    
    return true;
}

void PlayerUpdateAttacks(const fix32 CameraX, const fix32 CameraY)
{
    for (int i = 0; i < PlayerAttacksRunning; i++)
    {
        struct PlayerAttackData *p = &PlayerAttackList[i];
        
        switch(p->Type)
        {
            case PLAYER_ATTACK_SWIPE:   AttackSwipeTick(&PlayerAttackList[i]);   break;
            case PLAYER_ATTACK_BLAST:   AttackBlastTick(&PlayerAttackList[i]);   break;
            default:
            break;
        }

        p->Lifespan--;
        if (p->Lifespan == 0)
        {
            #if(DEBUG_MODE)
                int16_t sx = SPR_getPositionX(p->Graphic);
                int16_t sy = SPR_getPositionY(p->Graphic);
                kprintf("DEL ATK X:%i Y:%i", sx,sy);
            #endif
            SPR_setVisibility(PlayerAttackList[i].Graphic, HIDDEN);
            PlayerAttackList[i] = PlayerAttackList[--PlayerAttacksRunning];
            i--;
        }
        else
        {
            int16_t x = fix32ToInt(p->Position.x - CameraX);
            int16_t y = fix32ToInt(p->Position.y - CameraY);
            SPR_setPosition(p->Graphic, x, y);
        }
    }
}