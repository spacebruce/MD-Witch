#include "ObjectPlayer.h"
#include "../GameContext.h"

#define gravity FIX16(0.25)
#define acceleration FIX16(1.0)
#define jumpforce FIX16(5)
#define friction FIX16(0.7)

void ObjectPlayerUpdate(ObjectPlayer *object)
{
    const bool pressed_A = ((object->changed & BUTTON_A) == BUTTON_A);
    const bool pressed_B = ((object->changed & BUTTON_B) == BUTTON_B);
    const bool pressed_C = ((object->changed & BUTTON_C) == BUTTON_C);
    const bool pressed_left = ((object->changed & BUTTON_LEFT) == BUTTON_LEFT);
    const bool pressed_right = ((object->changed & BUTTON_RIGHT) == BUTTON_RIGHT);

    int FLoorY = 200;

    object->OnFloor = (fix32ToInt(object->Y) >= FLoorY);

    if(object->OnFloor)
    {         
        object->Y = intToFix32(FLoorY);
        // Apply friction
        if(abs(fix16ToInt(object->VelocityX)) < 1)
        {
            object->VelocityX = FIX16(0);
        } 
        else
        {
            object->VelocityX = fix16Mul(object->VelocityX, friction);
        }

        // Gravity 
        object->VelocityY = FIX16(0);
        if(pressed_A || pressed_B)  // Jump
        {
            object->VelocityY = FIX16(-5);  // Jump velocity
            object->OnFloor = false;        // Detatch from floor
        }

        if(pressed_left)
        {
            object->VelocityX = fix16Sub(object->VelocityX, acceleration);
        }
        else if (pressed_right)
        {
            object->VelocityX = fix16Add(object->VelocityX, acceleration);
        }
    }
    else
    {
        object->VelocityY = fix16Add(object->VelocityY, gravity);
    }
    object->X = fix32Add(object->X, fix16ToFix32(object->VelocityX));
    if(!object->OnFloor)
    {
        object->Y = fix32Add(object->Y, fix16ToFix32(object->VelocityY));
    }

    object->Base.x = fix32ToInt(object->X);
    object->Base.y = fix32ToInt(object->Y);
}

void ObjectPlayerCreate(ObjectPlayer *object)
{
    //
    object->VelocityX = FIX16(0);
    object->VelocityY = FIX16(0);
    object->OnFloor = FALSE;
}

void ObjectPlayerInput(ObjectPlayer *object, uint8_t state, uint8_t changed)
{
    object->state = state;
    object->changed = changed;
}