#include "EnemyBloober.h"
#include "../../GameContext.h"
#include <resources.h>

void EnemyBlooberInit(void* object)
{
    EnemyBloober* Bloober = (EnemyBloober*)object;

    Bloober->Base.spr = SPR_addSprite(&sprBloober, 0,0, TILE_ATTR(PAL_PLAYER, 0, FALSE, FALSE));
    SPR_setVisibility(Bloober->Base.spr, AUTO_FAST);
    
    Bloober->Base.spriteOffset.x = -16;
    Bloober->Base.spriteOffset.y = -32;

    //Bloober->Base.Collision.Type = CT_Circle;
    //Bloober->Base.Collision.Collision.Circle.Radius = 4;
}

void EnemyBlooberUpdate(void* object)
{
    EnemyBloober* Bloober = (EnemyBloober*)object;

    StageFunctionCollision col = GameContext.CurrentStage->Collision;

    int16_t x = fix32ToInt(Bloober->Base.x);
    int16_t y = fix32ToInt(Bloober->Base.y);
    while(col(x, y + 1) == false)
    {
        ++y;
    }
    Bloober->Base.y = FIX32(y);
}

void EnemyBlooberFree(void* object)
{
    EnemyBloober* Bloober = (EnemyBloober*)object;
    SPR_releaseSprite(Bloober->Base.spr);
}