#include "PlayerAttacks.h"
#include <resources.h>
#include "GameContext.h"

struct PlayerAttackData PlayerAttackList[PlayerAttacksMax];
u16 PlayerAttacksRunning = 0;

u16 AttackSpriteVRAM[9];

// Attack definitions
void AttackSwipeInit(struct PlayerAttackData* Attack)
{
    // PLACEHOLDER
    Attack->Lifespan = GameContext.Framerate / 10;
}
void AttackSwipeTick(struct PlayerAttackData* Attack)
{
    // PLACEHOLDER
}
void AttackBlastInit(struct PlayerAttackData* Attack)
{
    Sprite* spr = Attack->Graphic;
    SPR_setVRAMTileIndex(spr, AttackSpriteVRAM[0]);
    SPR_setVisibility(spr, AUTO_FAST);
    Attack->Lifespan = GameContext.Framerate * 2;

    switch(Attack->Direction)
    {
        case ATTACK_FACING_LEFT:    Attack->Vector = (Vect2D_f16){ FIX16(-4), FIX16(+0) };    break;
        case ATTACK_FACING_RIGHT:   Attack->Vector = (Vect2D_f16){ FIX16(+4), FIX16(+0) };    break;
        case ATTACK_FACING_UP:      Attack->Vector = (Vect2D_f16){ FIX16(+0), FIX16(-4) };    break;
        case ATTACK_FACING_DOWN:    Attack->Vector = (Vect2D_f16){ FIX16(+0), FIX16(+4) };    break;
    }
}
void AttackBlastTick(struct PlayerAttackData* Attack)
{
    Attack->Position.x += fix16ToFix32(Attack->Vector.x);
    Attack->Position.y += fix16ToFix32(Attack->Vector.y);
}

uint16_t PlayerInitAttacks(uint16_t VRAM)
{
    memset(PlayerAttackList, '\0', sizeof(PlayerAttackList));
    PlayerAttacksRunning = 0;
    // Graphics

    /*
        Allocate every sprite... Maybe fix later to recycle vram 
    */
    Animation* anim = sprProjectile.animations[0];
    kprintf("frames : %i", anim->numFrame);
    for(int16_t i = 0; i < anim->numFrame; i++)
    {
        TileSet* tileset = anim->frames[i]->tileset;
        VDP_loadTileSet(tileset, VRAM, DMA);
        AttackSpriteVRAM[i] = VRAM;
        VRAM += tileset->numTile;
        kprintf("tiles : %i", tileset->numTile);
    }

    /*
        Give every possible attack instance a sprite, but switch uploading OFF so they can share
        graphics resources
    */
    for(int16_t i = 0; i < PlayerAttacksMax; ++i)
    {
        struct PlayerAttackData* A = &PlayerAttackList[i];
        A->Graphic = SPR_addSpriteEx(&sprProjectile, -64, -64, TILE_ATTR_FULL(PAL_COMMON, TRUE, FALSE, FALSE, 0), SPR_FLAG_AUTO_VISIBILITY );
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
    Attack->Direction = Direction;
    Attack->Type = Type;

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
        if (p->Lifespan <= 0)
        {
            #if(DEBUG_MODE)
                const int16_t sx = SPR_getPositionX(p->Graphic);
                const int16_t sy = SPR_getPositionY(p->Graphic);
                kprintf("DEL ATK X:%i Y:%i", sx,sy);
            #endif
            SPR_setVisibility(PlayerAttackList[i].Graphic, HIDDEN);
            SPR_setPosition(PlayerAttackList[i].Graphic, -32, -32);
            memcpy(&PlayerAttackList[i], &PlayerAttackList[--PlayerAttacksRunning], sizeof(struct PlayerAttackData));
            i--;
        }
        else
        {
            const int16_t x = fix32ToInt(p->Position.x - CameraX);
            const int16_t y = fix32ToInt(p->Position.y - CameraY);
            SPR_setPosition(p->Graphic, x - 12, y - 12);
        }
    }
}