#include <genesis.h>
#include <resources.h>
#include "ObjectPlayer.h"
#include "../GameContext.h"

#define gravity FIX16(0.25)
#define air_acceleration FIX16(0.75)
#define acceleration FIX16(1.0)
#define friction FIX16(0.7)

// How many frames to stay in air after leaving ledge (*pal_speedup)
#define coyote_time (10)

#define JumpForce FIX16(-4)
#define JumpAdd FIX16(-0.5)
#define JumpTimeMin (5)
#define JumpTimeMax (10)

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
        else if(object->ButtonFrames[i] < (GameContext.Framerate * 2))  // don't overflow, just hold at 2 seconds
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

    bool Grounded = false;

    //  Very first thing - apply momentum from previous frame
    object->X = fix32Add(object->X, fix16ToFix32(object->VelocityX));
    object->Y = fix32Add(object->Y, fix16ToFix32(object->VelocityY));

    //
    s32 x = fix32ToInt(object->X);
    s32 y = fix32ToInt(object->Y);
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
    bool moved = false;
    do
    {
        x = fix32ToInt(object->X); 
        int budge = 0;
        if(vel > 0)
        {
            x_right = x + width;
            sens_top = col(x_right + 2, y_top);
            sens_mid = col(x_right + 2, y_mid);
            sens_low = col(x_right + 2, y_low);
            budge = -1;
        }
        if(vel < 0)
        {
            x_left = x - width;
            sens_top = col(x_left - 2, y_top);
            sens_mid = col(x_left - 2, y_mid);
            sens_low = col(x_left - 2, y_low);
            budge = 1;
        }
        if(sens_top || sens_mid || sens_low)
        {
            object->VelocityX = FIX16(0);
            x += budge;
            moved = true;
        }
        else
        {
            stuck = false;
        }
        ++its;
    }
    while(stuck && (its < 10));

    // If collided, set real coords to rounded
    if(moved)
        object->X = FIX32(x);

    // Floor sensor
    const bool sens_feet_left = col(x_left + 1,y + 1);
    const bool sens_feet_mid = col(x,y + 1);
    const bool sens_feet_right = col(x_right - 1,y + 1);
    object->OnFloor = ((sens_feet_left + sens_feet_mid + sens_feet_right) > 0);

    if(object->OnFloor)
    {         
        Grounded = true;
        //Set coyote timer. If we leave ground, it starts tickin down
        object->CoyoteFrames = fix16ToInt(fix16Mul(intToFix16(coyote_time), GameContext.Speedup));
           
        //  Just landed
        if(object->OnfloorLast == false)    
        {
            // Animation trigger?
            
            // Check if stuck in floor
            bool stuck = true;
            int its = 0;
            while(stuck && (its < 10))
            {
                const bool sens_stuck_left = col(x_left,y);
                const bool sens_stuck_mid = col(x,y);
                const bool sens_stuck_right = col(x_right,y);
                stuck = (sens_stuck_left + sens_stuck_mid + sens_stuck_right) > 0; 
                if(stuck)
                {
                    --y;
                }
                ++its;
            };
            object->Y = FIX32(y);
        }
    }
    else
    {
        object->VelocityY = fix16Add(object->VelocityY, gravity);
        // Player is not on floor
        // Pretend we're in air for a fraction of a second after leaving a ledge to make jumps feel better
        if(object->CoyoteFrames == 0)
        {
            Grounded = false;
        }
        else
        {        
            Grounded = true;
            (object->CoyoteFrames)--;   // Tick down
        }
    }

    if(Grounded)
    {
        if(pressed_B)  // Jump
        {
            object->VelocityY = JumpForce;  // Jump velocity
            object->OnFloor = false;        // Detatch from floor
            object->CoyoteFrames = 0;       // negate coyote mode immediately
            object->JumpHold = 0;
        }
    }
    else
    {
        if(pressed_B && object->JumpHold != 0xFF)
        {
            const u8 MinJumpHoldTime = fix16ToInt(fix16Mul(intToFix16(JumpTimeMin), GameContext.Speedup));
            const u8 MaxJumpHoldTime = fix16ToInt(fix16Mul(intToFix16(JumpTimeMax), GameContext.Speedup));
            const int vel = fix16ToInt(object->VelocityY);

            // If velocity pushing player up... Remember small numbers == higher up the screen
            // If held time is within bounds of effect
            if(vel < 0 && object->JumpHold >= MinJumpHoldTime && object->JumpHold < MaxJumpHoldTime)
            {
                object->VelocityY = fix16Add(object->VelocityY, JumpAdd);
            }
            object->JumpHold++;
        }
        else    // Let go of jump -> end button hold bonus
        {
            object->JumpHold = 0xFF;
        }
    }
    // If on floor or in coyote-mode
    if(object->OnFloor)
    {
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
        if(pressed_left)
        {
            object->VelocityX = fix16Sub(object->VelocityX, acceleration);
        }
        else if (pressed_right)
        {
            object->VelocityX = fix16Add(object->VelocityX, acceleration);
        }
    }
    else    // In air
    {
        object->VelocityX = fix16Mul(object->VelocityX, friction);
        if(pressed_left)
        {
            object->VelocityX = fix16Sub(object->VelocityX, air_acceleration);
        }
        else if (pressed_right)
        {
            object->VelocityX = fix16Add(object->VelocityX, air_acceleration);
        }
    }
    object->Base.x = fix32ToInt(object->X); // Sprite position
    object->Base.y = fix32ToInt(object->Y);
    object->OnfloorLast = Grounded;
}

void ObjectPlayerCreate(ObjectPlayer *object)
{
    //
    object->VelocityX = FIX16(0);
    object->VelocityY = FIX16(0);
    object->Base.spriteOffset.x = -24;
    object->Base.spriteOffset.y = -48;
    object->OnFloor = FALSE;
    object->CoyoteFrames = 0;
    // sprite
    object->Base.spr = SPR_addSprite(&sprPlayer, 0,0, TILE_ATTR(PAL_PLAYER, 0,false,false));
    PAL_setPalette(PAL_PLAYER, sprPlayer.palette->data, DMA);
}

void ObjectPlayerInput(ObjectPlayer *object, uint8_t changed)
{
    object->changed = changed;
}