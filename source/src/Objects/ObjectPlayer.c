#include <genesis.h>
#include <resources.h>
#include "ObjectPlayer.h"
#include "../GameContext.h"

#define PlayerAnimStand (0)
#define PlayerAnimTurn (0)
#define PlayerAnimWalk (1)

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

inline void ObjectPlayerUpdateSprite(ObjectPlayer* Player)
{
    // Countdown even if offscreen
    --(Player->AnimationTick);
    if(SPR_isVisible(Player->Base.spr, false))
    {
        if(Player->AnimationTick <= 0)
        {
            SPR_nextFrame(Player->Base.spr);
            Player->AnimationTick = 8;
        }
    }
}

void ObjectPlayerUpdate(void* object)
{
    ObjectPlayer* Player = (ObjectPlayer*)object;
    //  Check for input holds
    for(uint8_t i = 0; i < 8; ++i)
    {
        bool down = ((Player->changed & ButtonMask[i]) == ButtonMask[i]);
        if(down == false)
        {
            if(Player->ButtonFrames[i] == 0xFF || Player->ButtonFrames[i] == 0x00)
                Player->ButtonFrames[i] = 0;
            else
                Player->ButtonFrames[i] = 0xFF;
        }
        else if(Player->ButtonFrames[i] < (GameContext.Framerate * 2))  // don't overflow, just hold at 2 seconds
        {
            Player->ButtonFrames[i] = Player->ButtonFrames[i] + 1;
        }
    }
    
    const bool released_A = (Player->ButtonFrames[4] == 0xFF);
    const bool released_B = (Player->ButtonFrames[5] == 0xFF);
    const bool released_C = (Player->ButtonFrames[6] == 0xFF);
    const bool released_left = (Player->ButtonFrames[0] == 0xFF);
    const bool released_right = (Player->ButtonFrames[1] == 0xFF);
    const bool released_up = (Player->ButtonFrames[2] == 0xFF);
    const bool released_down = (Player->ButtonFrames[3] == 0xFF);

    const bool pressed_A = !released_A && (Player->ButtonFrames[4] > 0);
    const bool pressed_B = !released_B && (Player->ButtonFrames[5] > 0);
    const bool pressed_C = !released_C && (Player->ButtonFrames[6] > 0);
    const bool pressed_left = !released_left && (Player->ButtonFrames[0] > 0);
    const bool pressed_right = !released_right && (Player->ButtonFrames[1] > 0);
    const bool pressed_up = !released_up && (Player->ButtonFrames[2] > 0);
    const bool pressed_down = !released_down && (Player->ButtonFrames[3] > 0);

    bool Grounded = false;

    const s32 width = 20;
    const s32 halfwidth = width / 2;
    const s32 height = 40;

    // Cache current velocity for later
    const fix16 OldVelocityX = Player->VelocityX;
    const fix16 OldVelocityY = Player->VelocityY;

    // Apply momentum from frame
    if(GameContext.Speedup == FIX16(1.0)) // NTSC mode
    {
        Player->Base.x = Player->Base.x + fix16ToFix32(Player->VelocityX);
        Player->Base.y = Player->Base.y + fix16ToFix32(Player->VelocityY);
    }
    else    // PAL Mode
    {
        // implement speed correction here... 
        Player->Base.x = Player->Base.x + fix16ToFix32(Player->VelocityX);
        Player->Base.y = Player->Base.y + fix16ToFix32(Player->VelocityY);
    }

    //
    s32 x = fix32ToInt(Player->Base.x);
    s32 y = fix32ToInt(Player->Base.y);
    s32 x_left  = x - halfwidth;
    s32 x_right = x + halfwidth;
    s32 y_top = y - height;
    s32 y_mid = y - (height / 2);
    s32 y_low = y - 1;

    StageFunctionCollision col = GameContext.CurrentStage->Collision;

    // Walking into walls sensor
    bool sens_top, sens_mid, sens_low;

    bool stuck = true;  // assume stuck
    int its = 0;
    int velX = fix16ToInt(Player->VelocityX);
    int velXFrac = fix16ToRoundedInt(Player->VelocityX);
    int velY = fix16ToInt(Player->VelocityY);

    bool moved = false;
    do
    {
        x = fix32ToInt(Player->Base.x);
        int budge = 0;
        if(velX > 0 || velXFrac > 0)
        {
            x_right = x + halfwidth;
            sens_top = col(x_right + 2, y_top);
            sens_mid = col(x_right + 2, y_mid);
            sens_low = col(x_right + 2, y_low);
            budge = -1;
        }
        if(velX < 0 || velXFrac < 0)
        {
            x_left = x - halfwidth;
            sens_top = col(x_left - 2, y_top);
            sens_mid = col(x_left - 2, y_mid);
            sens_low = col(x_left - 2, y_low);
            budge = 1;
        }
        if(sens_top || sens_mid || sens_low)
        {
            Player->VelocityX = FIX16(0);
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
        Player->Base.x = FIX32(x);
    
    // Floor/Ceiling sensors
    if(velY >= 0)   
    {
        // If not moving or falling downwards - switch on feet sensors for standing on floor
        const bool sens_feet_left  = col(x_left + 1,y + 1);
        const bool sens_feet_mid   = col(x,y + 1);
        const bool sens_feet_right = col(x_right - 1,y + 1);
        Player->OnFloor = (sens_feet_left | sens_feet_mid | sens_feet_right);
    }
    else
    {
        // Head sensor. If Ascending, check head isn't getting bonked
        bool bonk_left  = col(x_left + 1,y - height);
        bool bonk_mid   = col(x,y - height);
        bool bonk_right = col(x_right - 1,y - height);
        bool bonking = (bonk_left | bonk_mid | bonk_right);
        
        if(bonking)
        {
            u16 its = 0;
            while(bonking && (its < 10))  // Push out of ceiling
            {
                ++y;        // Shift temp coordinate down
                ++its;
                bonk_left  = col(x_left + 1,y - height);
                bonk_mid   = col(x,y - height);
                bonk_right = col(x_right - 1,y - height);
                bonking = (bonk_left | bonk_mid | bonk_right);
            }
            // move actual player coords if bonked
            // Might want to play a thonk sound effect here. 
            Player->VelocityY = FIX16(0);
            Player->Base.y = FIX32(y);
        }
    }

    if(Player->OnFloor)
    {         
        Grounded = true;
        //Set coyote timer. If we leave ground, it starts tickin down
        Player->CoyoteFrames = fix16ToInt(fix16Mul(intToFix16(coyote_time), GameContext.Speedup));
           
        //  Just landed
        if(Player->OnfloorLast == false)    
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
            Player->Base.y = FIX32(y);
        }
    }
    else
    {
        Player->VelocityY = Player->VelocityY + gravity;
        // Player is not on floor
        // Pretend we're in air for a fraction of a second after leaving a ledge to make jumps feel better
        if(Player->CoyoteFrames == 0)
        {
            Grounded = false;
        }
        else
        {        
            Grounded = true;
            (Player->CoyoteFrames)--;   // Tick down
        }
    }

    if(Grounded)
    {
        if(pressed_B)  // Jump
        {
            Player->VelocityY = JumpForce;  // Jump velocity
            Player->OnFloor = false;        // Detatch from floor
            Player->CoyoteFrames = 0;       // negate coyote mode immediately
            Player->JumpHold = 0;
        }
    }
    else
    {
        if(pressed_B && Player->JumpHold != 0xFF)
        {
            const u8 MinJumpHoldTime = fix16ToInt(fix16Mul(intToFix16(JumpTimeMin), GameContext.Speedup));
            const u8 MaxJumpHoldTime = fix16ToInt(fix16Mul(intToFix16(JumpTimeMax), GameContext.Speedup));
            const int vel = fix16ToInt(Player->VelocityY);

            // If velocity pushing player up... Remember small numbers == higher up the screen
            // If held time is within bounds of effect
            if(vel < 0 && Player->JumpHold >= MinJumpHoldTime && Player->JumpHold < MaxJumpHoldTime)
            {
                Player->VelocityY = Player->VelocityY + JumpAdd;
            }
            Player->JumpHold++;
        }
        else    // Let go of jump -> end button hold bonus
        {
            Player->JumpHold = 0xFF;
        }
    }
    // If on floor or in coyote-mode
    if(Player->OnFloor)
    {
        // Apply friction. If < 0, stop.
        if((abs(fix16ToInt(Player->VelocityX)) <= 1) && (!pressed_left && !pressed_right))
        {
            Player->VelocityX = FIX16(0);
        }
        else
        {
            Player->VelocityX = fix16Mul(Player->VelocityX, friction);
        }

        // Gravity 
        Player->VelocityY = FIX16(0);
        if(pressed_left)
        {
            Player->VelocityX = Player->VelocityX - acceleration;
            SPR_setAnim(Player->Base.spr, PlayerAnimWalk);
        }
        else if (pressed_right)
        {
            Player->VelocityX = Player->VelocityX + acceleration;
            SPR_setAnim(Player->Base.spr, PlayerAnimWalk);
        }
    }
    else    // In air
    {
        Player->VelocityX = fix16Mul(Player->VelocityX, friction);
        if(pressed_left)
        {
            Player->VelocityX = Player->VelocityX - air_acceleration;
        }
        else if (pressed_right)
        {
            Player->VelocityX = Player->VelocityX + air_acceleration;
        }
    }

    if(Player->VelocityX > 0)
        SPR_setHFlip(Player->Base.spr, false);
    else if (Player->VelocityX < 0)
        SPR_setHFlip(Player->Base.spr, true);

    if(Player->VelocityX == 0)
        SPR_setAnim(Player->Base.spr, PlayerAnimStand);

    ObjectPlayerUpdateSprite(object);

    Player->OnfloorLast = Grounded;
    
    //SetCollisionRectangleAligned(&Player->Base.Collision, fix32ToInt(Player->Base.x), fix32ToInt(Player->Base.y)-2, 20, 40, BottomMiddle);
    //SetCollisionRectangle(&Player->Base.Collision, fix32ToInt(Player->Base.x) - 20, fix32ToInt(Player->Base.y) + 24, 20, 40);
}

void ObjectPlayerInit(void* object)
{
    ObjectPlayer* Player = (ObjectPlayer*)object;
    //
    ObjectPlayerInput(Player, 0x00);
    //
    Player->Health = 100;
    Player->MaxHealth = 100;
    //
    Player->AnimationState = AnimaionStanding;
    Player->AnimationTick = 0;
    // Physics
    Player->VelocityX = FIX16(0);
    Player->VelocityY = FIX16(0);
    Player->OnFloor = FALSE;
    Player->CoyoteFrames = 0;
    // sprite
    Player->Base.spr = SPR_addSprite(&sprPlayer, 0,0, TILE_ATTR(PAL_PLAYER, 0,false,false));
    
    SPR_setVisibility(Player->Base.spr, AUTO_FAST);
    Player->Base.spriteOffset.x = -24;
    Player->Base.spriteOffset.y = -48;

    u16 pal[16];
    for(int i = 0; i < 16; ++i)
        pal[i] = sprPlayer.palette->data[i] / 2;

    memcpy(&GameContext.palette[PAL_PLAYER], sprPlayer.palette->data, 16 * 2);
    memcpy(&GameContext.paletteEffect[PAL_PLAYER], pal, 16 * 2);
}

void ObjectPlayerInput(ObjectPlayer *Player, uint8_t changed)
{
    Player->changed = changed;
}

void ObjectPlayerFree(void* object)
{
    ObjectPlayer* Player = (ObjectPlayer*)object;
    SPR_releaseSprite(Player->Base.spr);
}