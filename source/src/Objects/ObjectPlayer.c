#include "ObjectPlayer.h"
#include "../GameContext.h"

#define gravity FIX16(0.25)
#define acceleration FIX16(1.0)
#define jumpforce FIX16(5)
#define friction FIX16(0.7)


void ObjectPlayerUpdate(ObjectPlayer *object)
{
    bool pressed_A = ((object->changed & BUTTON_A) == BUTTON_A);
    bool pressed_B = ((object->changed & BUTTON_B) == BUTTON_B);
    bool pressed_C = ((object->changed & BUTTON_C) == BUTTON_C);
    bool pressed_left = ((object->changed & BUTTON_LEFT) == BUTTON_LEFT);
    bool pressed_right = ((object->changed & BUTTON_RIGHT) == BUTTON_RIGHT);

    const s32 x = object->Base.x;
    const s32 y = object->Base.y; 
    const s32 x_left  = x - 16;
    const s32 x_right = x + 16;
    const s32 y_top = y - 32;

    StageFunctionCollision col = GameContext.CurrentStage->Collision;

    bool sens_feet_left = col(x_left,y + 1);
    bool sens_feet_mid = col(x,y + 1);
    bool sens_feet_right = col(x_right,y + 1);
    bool sens_left = col(x_left, y - 16);
    bool sens_right = col(x_right, y - 16);

    if(pressed_left & sens_left)    pressed_left = false;
    if(pressed_right & sens_right)  pressed_right = false;

    object->OnFloor = (sens_feet_left + sens_feet_mid + sens_feet_right) > 2;

    if(object->OnFloor)
    {         
        //object->Y = intToFix32(FLoorY);
        // Apply friction
        if((abs(fix16ToInt(object->VelocityX)) <= 1) && (!pressed_left && !pressed_right))
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