#include "Player.h"
#include <genesis.h>
#include <resources.h>

void PlayerInput(PlayerType* P, u16 Joy, u16 Changed, u16 State)
{
}
void PlayerInit(PlayerType* P)
{
    P->sprite = SPR_addSprite(&gfx_cursor, 32,32,  TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    //PAL_setPalette(PAL1, P->sprite.palette->data, DMA);
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