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

enum 
{
    BUTTON_ID_LEFT = 0, BUTTON_ID_RIGHT = 1, BUTTON_ID_UP = 2, BUTTON_ID_DOWN = 3,
    BUTTON_ID_A = 4, BUTTON_ID_B = 5, BUTTON_ID_C = 7, BUTTON_ID_START = 8;
} BUTTON_ID;

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
    
    const bool released_left = (Player->ButtonFrames[0] == 0xFF);
    const bool released_right = (Player->ButtonFrames[1] == 0xFF);
    const bool released_up = (Player->ButtonFrames[2] == 0xFF);
    const bool released_down = (Player->ButtonFrames[3] == 0xFF);
    const bool released_A = (Player->ButtonFrames[4] == 0xFF);
    const bool released_B = (Player->ButtonFrames[5] == 0xFF);
    const bool released_C = (Player->ButtonFrames[6] == 0xFF);

    const bool pressed_left = !released_left && (Player->ButtonFrames[0] > 0);
    const bool pressed_right = !released_right && (Player->ButtonFrames[1] > 0);
    const bool pressed_up = !released_up && (Player->ButtonFrames[2] > 0);
    const bool pressed_down = !released_down && (Player->ButtonFrames[3] > 0);
    const bool pressed_A = !released_A && (Player->ButtonFrames[4] > 0);
    const bool pressed_B = !released_B && (Player->ButtonFrames[5] > 0);
    const bool pressed_C = !released_C && (Player->ButtonFrames[6] > 0);

    // Control remapping
    const bool pressed_jump = pressed_A;
    const bool pressed_attack = pressed_B;
    const bool pressed_shoot = pressed_C;
    
    //
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
        Player->Base.x = fix32Add(Player->Base.x, fix16ToFix32(Player->VelocityX));
        Player->Base.y = fix32Add(Player->Base.y, fix16ToFix32(Player->VelocityY));
    }
    else    // PAL Mode
    {
        // implement speed correction here... 
        Player->Base.x = fix32Add(Player->Base.x, fix16ToFix32(Player->VelocityX));
        Player->Base.y = fix32Add(Player->Base.y, fix16ToFix32(Player->VelocityY));
    }

    // 
    s32 x = fix32ToInt(Player->Base.x);
    s32 y = fix32ToInt(Player->Base.y);
    s32 x_left  = x - halfwidth;
    s32 x_right = x + halfwidth;
    s32 y_top = y - height;
    s32 y_mid = y - (height / 2);
    s32 y_low = y - 1;
    
    //
    const int VelocityY = fix16ToInt(Player->VelocityY);
    const int VelocityYFrac = fix16ToRoundedInt(Player->VelocityY);
    const int VelocityX = fix16ToInt(Player->VelocityX);
    const int VelocityXFrac = fix16ToRoundedInt(Player->VelocityX);

    const bool StateChanged = (Player->State != Player->StateLast); 

    // Control code
    bool finish = false;
    while(!finish)
    {
        finish = true;  // Only run once most of the time
        switch(Player->State)
        {
            case STATE_STANDING:
            {
                if(!Player->OnFloor)
                    Player->State = STATE_FALLING;
                if(pressed_left || pressed_right)
                    Player->State = STATE_WALKING;
                if(pressed_jump)   // Can supercede a STATE_WALKING switch
                    Player->State = STATE_JUMPING;
                if(pressed_attack)
                    Player->State = STATE_HITTING;
                if(pressed_shoot)
                    Player->State = STATE_SHOOTING;
            }   break;
            case STATE_WALKING:
            {
                if(released_left || released_right)
                    Player->State = STATE_STANDING;
                if(pressed_jump)
                    Player->State = STATE_JUMPING;
                if(!Player->OnFloor)    // Walk of a ledge/floor falls away
                    Player->State = STATE_FALLING;
            }   break;
            case STATE_JUMPING:
                if(Player->Bonked)
                    Player->State = STATE_BONKED;
                if(VelocityY < 0)
                    Player->State = STATE_FALLING;
            break;
            case STATE_FALLING:
                if(Player->OnFloor)
                {
                    if(Moving)  Player->State = STATE_WALKING;
                    else        Player->State = STATE_LANDING;
                }
            break;
            case STATE_LANDING:
                if(AnimationOver || pressed_any)
                {
                    Player->State = STATE_WAL
                    finish = false;
                };
            break;
            case STATE_BONKED:
            break;
            case STATE_SKIDDING:
            break;
            case STATE_CROUCHING:
            break;
            case STATE_CRAWLING:
            break;
            case STATE_HITTING:
            break;
            case STATE_SHOOTING:
            break;
            
        }
    }
    Player->StateLast = Player->State;
    
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

    ObjectPlayerUpdateSprite(object);

    Player->OnfloorLast = Grounded;
    
    SetCollisionRectangleAligned(&Player->Base.Collision, fix32ToInt(Player->Base.x), fix32ToInt(Player->Base.y), 20, 40, BottomMiddle);
    //SetCollisionRectangle(&Player->Base.Collision, fix32ToInt(Player->Base.x) - 20, fix32ToInt(Player->Base.y) + 24, 20, 40);
}

void ObjectPlayerInit(void* object)
{
    ObjectPlayer* Player = (ObjectPlayer*)object;

    Player->Health = 100;
    Player->MaxHealth = 100;
    //
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
    PAL_setPalette(PAL_PLAYER, sprPlayer.palette->data, DMA);
}

void ObjectPlayerInput(ObjectPlayer *Player, uint8_t changed)
{
    Player->changed = changed;
}