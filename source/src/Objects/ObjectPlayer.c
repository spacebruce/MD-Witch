#include <genesis.h>
#include <resources.h>
#include "ObjectPlayer.h"
#include "../GameContext.h"

#define gravity FIX16(0.25)
#define acceleration FIX16(1.0)
#define jumpforce FIX16(5)
#define friction FIX16(0.7)

const uint16_t ButtonMask[] = 
{
    BUTTON_LEFT, BUTTON_RIGHT, BUTTON_UP, BUTTON_DOWN,
    BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_START,
};

void ObjectPlayerUpdate(ObjectPlayer *object)
{
    //  Check for input holds
    for(uint8_t i = 0; i < 8; ++i)
    {
        bool down = ((object->changed & ButtonMask[i]) == ButtonMask[i]);
        if(down == false)
        {
            if(object->ButtonFrames[i] == 0xFF || object->ButtonFrames[i] == 0x00)
                object->ButtonFrames[i] = 0;
            else
                object->ButtonFrames[i] = 0xFF;
        }
        else if(object->ButtonFrames[i] < 100)  // don't overflow, just hold at 100
        {
            object->ButtonFrames[i] = object->ButtonFrames[i] + 1;
        }
    }
    
    const bool released_A = (object->ButtonFrames[4] == 0xFF);
    const bool released_B = (object->ButtonFrames[5] == 0xFF);
    const bool released_C = (object->ButtonFrames[6] == 0xFF);
    const bool released_left = (object->ButtonFrames[0] == 0xFF);
    const bool released_right = (object->ButtonFrames[1] == 0xFF);
    const bool released_up = (object->ButtonFrames[2] == 0xFF);
    const bool released_down = (object->ButtonFrames[3] == 0xFF);

    const bool pressed_A = !released_A && (object->ButtonFrames[4] > 0);
    const bool pressed_B = !released_B && (object->ButtonFrames[5] > 0);
    const bool pressed_C = !released_C && (object->ButtonFrames[6] > 0);
    const bool pressed_left = !released_left && (object->ButtonFrames[0] > 0);
    const bool pressed_right = !released_right && (object->ButtonFrames[1] > 0);
    const bool pressed_up = !released_up && (object->ButtonFrames[2] > 0);
    const bool pressed_down = !released_down && (object->ButtonFrames[3] > 0);

    //
    s32 x = object->Base.x;
    s32 y = object->Base.y; 
    const s32 width = 10;
    s32 x_left  = x - width;
    s32 x_right = x + width;
    s32 y_top = y - 32;
    s32 y_mid = y - 16;
    s32 y_low = y - 1;

    StageFunctionCollision col = GameContext.CurrentStage->Collision;

    // Walking into walls sensor
    bool sens_top, sens_mid, sens_low;

    bool stuck = true;  // assume stuck
    int its = 0;
    int vel = fix16ToInt(object->VelocityX);
    if(vel != 0)
    {
        do
        {
            x = object->Base.x;
            int budge = 0;
            if(vel > 0)
            {
                x_right = x + width;
                sens_top = col(x_right, y_top);
                sens_mid = col(x_right, y_mid);
                sens_low = col(x_right, y_low);
                budge = -1;
            }
            if(vel < 0)
            {
                x_left = x - width;
                sens_top = col(x_left, y_top);
                sens_mid = col(x_left, y_mid);
                sens_low = col(x_left, y_low);
                budge = 1;
            }
            if(sens_top || sens_mid || sens_low)
            {
                object->VelocityX = FIX16(0);
                x += budge;
                object->X = FIX32(x);
            }
            else
            {
                stuck = false;
            }
            ++its;
        }
        while(stuck && (its < 10));
    }


    // Floor sensor
    const bool sens_feet_left = col(x_left + 1,y + 1);
    const bool sens_feet_mid = col(x,y + 1);
    const bool sens_feet_right = col(x_right - 1,y + 1);
    object->OnFloor = (sens_feet_left + sens_feet_mid + sens_feet_right) > 1;

    if(object->OnFloor)
    {         
        //  Just landed
        if(object->OnfloorLast == false)    
        {
            // Animation trigger?
            
            // Check if stuck in floor
            bool stuck = true;
            int its = 0;
            while(stuck && (its < 10))
            {
                const bool sens_stuck_left = col(x_left + 2,y);
                const bool sens_stuck_mid = col(x,y);
                const bool sens_stuck_right = col(x_right - 2,y);
                stuck = (sens_stuck_left + sens_stuck_mid + sens_stuck_right) > 0;
                if(stuck)
                {
                    --y;
                }
                ++its;
            };
            object->Y = FIX32(y);
        }

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
        if(pressed_B)  // Jump
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
    object->OnfloorLast = object->OnFloor;
}

void ObjectPlayerCreate(ObjectPlayer *object)
{
    //
    object->VelocityX = FIX16(0);
    object->VelocityY = FIX16(0);
    object->Base.spriteOffset.x = -24;
    object->Base.spriteOffset.y = -48;
    object->OnFloor = FALSE;
    // sprite
    object->Base.spr = SPR_addSprite(&sprPlayer, 0,0, TILE_ATTR(PAL_PLAYER, 0,false,false));
}

void ObjectPlayerInput(ObjectPlayer *object, uint8_t state, uint8_t changed)
{
    object->state = state;
    object->changed = changed;
}