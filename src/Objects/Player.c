#include "Player.h"

void PlayerInput(Player* P, u16 Joy, u16 Changed, u16 State)
{
}
void PlayerInit(Player* P)
{
}
void PlayerFree(Player* P)
{
}
void PlayerUpdate(Player* P)
{
    P->Base.X += P->MomentumX;
    P->Base.Y += P->MomentumY;
}
void PlayerDraw(Player* P)
{
    
}