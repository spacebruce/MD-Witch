#include "ObjectPlayer.h"
#include "../GameContext.h"

void updateObjectPlayer(ObjectPlayer *object)
{
    object->OnFloor = (fix32ToInt(object->Base.y) > 200);
    if(object->OnFloor)
    {
        object->VelocityY = FIX16(0);
        if((object->changed & BUTTON_A) == BUTTON_A)
        {
            object->VelocityY = FIX16(-5);
        }
        
        object->VelocityX = fix16Div(object->VelocityX, FIX16(1.1));

        if((object->state & BUTTON_LEFT) == BUTTON_LEFT)
        {
            object->VelocityX = fix16Add(object->VelocityX, FIX16(-1));
        }
        else if ((object->state & BUTTON_RIGHT) == BUTTON_RIGHT)
        {
            object->VelocityX = fix16Add(object->VelocityX, FIX16(1));
        }
    }
    else
    {
        fix16 Gravity = FIX16(0.1);
        if(GameContext.Speedup != FIX16(1))
        {
            Gravity = fix16Mul(Gravity, GameContext.Speedup);
        }
        object->VelocityY = fix16Add(object->VelocityY, Gravity);
    }
    object->Base.x = fix32Add(object->Base.x, fix16ToFix32(object->VelocityX));
    object->Base.y = fix32Add(object->Base.y, fix16ToFix32(object->VelocityY));
}

void createObjectPlayer(ObjectPlayer *object)
{
    //
    object->VelocityX = FIX16(0);
    object->VelocityY = FIX16(0);
    object->OnFloor = FALSE;
}

void inputObjectPlayer(ObjectPlayer *object, uint8_t state, uint8_t changed)
{
    object->state = state;
    object->changed = changed;
}