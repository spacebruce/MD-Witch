#include "Player.h"
#include <genesis.h>
#include <resources.h>

#include "../defines.h"

void PlayerInput(PlayerType* P, u16 Joy, u16 Changed, u16 State)
{
    if(Joy != JOY_1)    
        return;
    
    if(State & BUTTON_RIGHT)
    {
        P->MomentumX = FIX16(1.5);
    }
    else if(State & BUTTON_LEFT)
    {
        P->MomentumX = FIX16(-1.5);
    }
    else if ((Changed & BUTTON_LEFT) | (Changed & BUTTON_RIGHT))
        P->MomentumX = FIX16(0);

    
}
void PlayerInit(PlayerType* P)
{
    // graphics
    P->sprite = SPR_addSprite(&sprPlayer, 32,32, TILE_ATTR(PLAYER_PALETTE, 1, FALSE, FALSE));
    PAL_setPalette(PLAYER_PALETTE, sprPlayer.palette->data, DMA);   // do this at drawtime for flashing/effects?
    //
    P->Lives = 1;
}
void PlayerFree(PlayerType* P)
{
    SPR_releaseSprite(P->sprite);
}
void PlayerUpdate(PlayerType* P)
{
    P->Base.X += fix16ToInt(P->MomentumX);
    P->Base.Y += fix16ToInt(P->MomentumY);
    SPR_setHFlip(P->sprite, P->MomentumX < 0);
}
void PlayerDraw(PlayerType* P)
{
    SPR_setPosition(P->sprite, P->Base.X,P->Base.Y);
}