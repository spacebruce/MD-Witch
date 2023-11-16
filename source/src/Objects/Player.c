#include "Player.h"
#include <genesis.h>
#include <resources.h>

#include "../defines.h"

void PlayerInput(PlayerType* P, u16 Joy, u16 Changed, u16 State)
{
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
    P->Base.X = random() % 400;
    P->Base.Y = random() % 260;
    //P->Base.X += P->MomentumX;
    //P->Base.Y += P->MomentumY;
}
void PlayerDraw(PlayerType* P)
{
    SPR_setPosition(P->sprite, P->Base.X,P->Base.Y);
}