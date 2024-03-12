#include <genesis.h>
#include <resources.h>
#include "ObjectPlayer.h"
#include "../GameContext.h"

#include "../Types/NumericHelpers.h"

#include "../PlayerAttacks.h"

#define gravity FIX16(0.25)
#define air_acceleration FIX16(0.75)
#define acceleration FIX16(1.0)
#define friction FIX16(0.7)

// How many frames to stay in air after leaving ledge (*pal_speedup)
#define coyote_time (10)

#define JumpForce FIX16(-4)
#define JumpAdd FIX16(-0.15)
#define JumpTimeMin (5)
#define JumpTimeMax (10)

const uint16_t ButtonMask[] = 
{
    BUTTON_LEFT, BUTTON_RIGHT, BUTTON_UP, BUTTON_DOWN,
    BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_START,
};

void ObjectPlayerInput(ObjectPlayer *Player, uint8_t Changed)
{
    struct PlayerController* Controller = &Player->Controller;
    Controller->Changed = Changed;
}

void ObjectPlayerProcessInputs(struct PlayerController* Controller)
{
    //  Check for input holds & releases
    for(uint8_t i = 0; i < 8; ++i)
    {
        bool down = ((Controller->Changed & ButtonMask[i]) == ButtonMask[i]);
        if(down == false)
        {
            if(Controller->ButtonFrames[i] == 0xFF || Controller->ButtonFrames[i] == 0x00)
                Controller->ButtonFrames[i] = 0;
            else
                Controller->ButtonFrames[i] = 0xFF;
        }
        else if(Controller->ButtonFrames[i] < (GameContext.Framerate * 2))  // don't overflow, just hold at 2 seconds
        {
            Controller->ButtonFrames[i] = Controller->ButtonFrames[i] + 1;
        }
    }
    
    // Unmangle!
    Controller->Release_Left = (Controller->ButtonFrames[0] == 0xFF);
    Controller->Release_Right = (Controller->ButtonFrames[1] == 0xFF);
    Controller->Release_Up = (Controller->ButtonFrames[2] == 0xFF);
    Controller->Release_Down = (Controller->ButtonFrames[3] == 0xFF);

    Controller->Release_Jump = (Controller->ButtonFrames[4] == 0xFF);   // A
    Controller->Release_Shoot = (Controller->ButtonFrames[5] == 0xFF);  // B
    Controller->Release_C = (Controller->ButtonFrames[6] == 0xFF);      // C

    Controller->Pressed_Left = !(Controller->Release_Left) && (Controller->ButtonFrames[0] > 0);
    Controller->Pressed_Right = !(Controller->Release_Right) && (Controller->ButtonFrames[1] > 0);
    Controller->Pressed_Up = !(Controller->Release_Up) && (Controller->ButtonFrames[2] > 0);
    Controller->Pressed_Down = !(Controller->Release_Down) && (Controller->ButtonFrames[3] > 0);

    Controller->Pressed_Jump = !(Controller->Release_Jump) && (Controller->ButtonFrames[4] > 0);    // A
    Controller->Pressed_Shoot = !(Controller->Release_Shoot) && (Controller->ButtonFrames[5] > 0);  // B
    Controller->Pressed_C = !(Controller->Release_C) && (Controller->ButtonFrames[6] > 0);          // C

    Controller->Moving = (Controller->Pressed_Left || Controller->Pressed_Right);

    if(Controller->Moving)
    {
        if(Controller->Pressed_Left)    Controller->WalkDir = 0;
        if(Controller->Pressed_Right)   Controller->WalkDir = 1;
    }
}

inline void ObjectPlayerUpdateSprite(ObjectPlayer* Player, const bool StateBegin)
{
    const int16_t OldAnimation = Player->LastAnimationState;
    int16_t NewAnimation;

    if(Player->VelocityX > FIX16(0))
        SPR_setHFlip(Player->Base.spr, false);
    else if (Player->VelocityX < FIX16(0))
        SPR_setHFlip(Player->Base.spr, true);

    NewAnimation = Player->AnimationState;

    if(NewAnimation != 0xFF && (NewAnimation != OldAnimation))
    {
        SPR_setAnim(Player->Base.spr, NewAnimation);
    }
    
    Player->LastAnimationState = Player->AnimationState;
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

void ObjectPlayerStateStanding(ObjectPlayer* Player)
{
    Player->AnimationState = PlayerAnimStand;

    // State exits
    if(Player->Controller.Moving)
    {
        if(Player->Controller.Pressed_Left)
            Player->Controller.WalkDir = 0;
        if(Player->Controller.Pressed_Right)
            Player->Controller.WalkDir = 1;
        Player->State = PlayerWalking;
        return;
    }
    if(!Player->OnFloor)
    {
        Player->State = PlayerFalling;
        return;
    }
    if(Player->Controller.Pressed_Jump)
    {
        Player->State = PlayerJumping;
        return;
    }
    if(Player->Controller.Pressed_Shoot)
    {
        Player->State = PlayerShooting;
        return;
    }
}
void ObjectPlayerStateWalking(ObjectPlayer* Player)
{
    Player->AnimationState = PlayerAnimWalk;

    // State exits
    if(!Player->Controller.Moving)
    {
        Player->State = PlayerStanding;
        Player->CoyoteMode = false;
        return;
    }
    if(!Player->OnFloor)
    {
        Player->CoyoteFrames--;
        Player->CoyoteMode = true;
        if(Player->CoyoteFrames <= 0)
        {
            Player->State = PlayerFalling;
            Player->CoyoteMode = false;
            return;
        }
    }
    else
    {
        Player->CoyoteMode = false;
        Player->CoyoteFrames = fix16ToInt(fix16Mul(intToFix16(coyote_time), GameContext.Speedup));
    }
    if(Player->Controller.Pressed_Jump)
    {
        Player->State = PlayerJumping;
        Player->OnFloor = false;
        Player->CoyoteMode = false;
        return;
    }
    if(Player->Controller.Pressed_Shoot)
    {
        Player->State = PlayerShooting;
        Player->CoyoteMode = false;
        return;
    }
    
    switch(Player->Controller.WalkDir)
    {
        case 0: Player->VelocityX = Player->VelocityX - acceleration;   break;
        case 1: Player->VelocityX = Player->VelocityX + acceleration;   break;
    }
}
void ObjectPlayerStateJumping(ObjectPlayer* Player)
{
    Player->AnimationState = PlayerAnimStand;
    // Jump logic
    if(Player->StateFrame == 0)
    {
        #if(DEBUG_MODE)        
            kprintf("UP!");
        #endif
        Player->VelocityY = JumpForce;
        Player->OnFloor = false;        // Detatch from floor
        Player->JumpHold = 0;           // Hold timer start
    }
    else
    {
        if(Player->Controller.Release_Jump) // Cancel hold boost
        {
            Player->JumpHold = 0xFF;
        }
    }

    if(Player->VelocityY >= FIX16(0))
    {
        #if(DEBUG_MODE)        
            kprintf("FALL!");
        #endif
        Player->State = PlayerFalling;
        return;
    }

    if(Player->JumpHold != 0xFF)
    {
        Player->VelocityY += JumpAdd;
        // Can't hold for 2 long
        if(Player->JumpHold >= fix16Div(JumpTimeMax, GameContext.Speedup))
        {
            Player->JumpHold = 0xFF;
        }
        ++Player->JumpHold;
    }
}
void ObjectPlayerStateFalling(ObjectPlayer* Player)
{
    Player->AnimationState = PlayerAnimStand;
    //
    if(Player->OnFloor)
    {
        if(Player->VelocityX != FIX16(0))
            Player->State = PlayerWalking;
        else
            Player->State = PlayerStanding;
    }
    //
}

void ObjectPlayerStateShooting(ObjectPlayer* Player)
{
    Player->AnimationState = PlayerAnimShoot;

    //
    if((Player->StateFrame % 10) == 0)
    {
        fix16 xoff = Player->Controller.WalkDir ? FIX32(20) : FIX32(-20);
        fix16 yoff = FIX32(-20);
        PlayerCreateAttack(Player->x + xoff, Player->y + yoff, Player->Controller.WalkDir ? ATTACK_FACING_RIGHT : ATTACK_FACING_LEFT, PLAYER_ATTACK_BLAST);
    }
    
    if(Player->Controller.Release_Shoot)
    {
        if(Player->VelocityX != FIX16(0))
            Player->State = PlayerWalking;
        else
            Player->State = PlayerStanding;
    }
}
void ObjectPlayerStateHurt(ObjectPlayer* Player)
{
    Player->AnimationState = PlayerAnimHurt;

    // Entry
    if(Player->StateFrame == 0)
    {
        // Physics stuff
    }
    
    if(Player->StateFrame >= fix16ToInt(fix16Mul(FIX16(15),GameContext.Speedup)))
    {
        SPR_setPalette(Player->Base.spr, PAL_PLAYER);
        if(Player->OnFloor) 
            Player->State = PlayerStanding;
        else
            Player->State = PlayerFalling;
    }
    else
    {
        SPR_setPalette(Player->Base.spr, (Player->StateFrame / 3) % 4);
    }
}

void ObjectPlayerUpdate(void* object)
{
    ObjectPlayer* Player = (ObjectPlayer*)object;

    ObjectPlayerProcessInputs(&Player->Controller);

    const s32 width = 20;
    const s32 halfwidth = width / 2;
    const s32 height = 40;

    int16_t x = fix32ToInt(Player->x);
    int16_t y = fix32ToInt(Player->y);
    
    int16_t x_left  = x - halfwidth;
    int16_t x_right = x + halfwidth;
    int16_t y_top = y - height;
    int16_t y_mid = y - (height / 2);
    int16_t y_low = y - 10;

    bool stuck;
    
    int16_t velX = fix16ToInt(Player->VelocityX);
    int16_t velXFrac = fix16ToRoundedInt(Player->VelocityX);
    int16_t velY = fix16ToInt(Player->VelocityY);

    StageFunctionCollision col = GameContext.CurrentStage->Collision;

    // Sideways collisions
    if(velX !=0 || velXFrac != 0)
    {
        bool sens_top, sens_mid, sens_low;
        bool moved = false;
        
        int16_t xsens = x;
        int16_t budge = 0;
        if(velX > 0 || velXFrac > 0)
        {
            budge = -1;
            xsens += halfwidth;
        }
        else
        {
            budge = 1;
            xsens -= halfwidth;
        }

        stuck = (col(xsens, y_top) | col(xsens, y_mid)) > 0;
        if(stuck)
        {
            int16_t its = 0;
            while(stuck && (its < 10))
            { 
                xsens += budge;
                x += budge;
                stuck = (col(xsens, y_top) | col(xsens, y_mid)) > 0;
                ++its;
            }
            Player->VelocityX = FIX16(0);
            Player->x = FIX32(x);
        }
        else
        {
            // Apply momentum from frame
            if(GameContext.Speedup == FIX16(1.0)) // NTSC mode
            {
                Player->x = Player->x + fix16ToFix32(Player->VelocityX);
            }
            else    // PAL Mode
            {
                // implement speed correction here... 
                Player->x = Player->x + fix16ToFix32(Player->VelocityX);
            }
        }
    }

    if(GameContext.Speedup == FIX16(1.0)) // NTSC mode
        Player->y = Player->y + fix16ToFix32(Player->VelocityY);
    else
        Player->y = Player->y + fix16ToFix32(Player->VelocityY);

    switch(Player->State)
    {
        case PlayerStanding:    ObjectPlayerStateStanding(Player);  break;
        case PlayerWalking:     ObjectPlayerStateWalking(Player);   break;
        case PlayerJumping:     ObjectPlayerStateJumping(Player);   break;
        case PlayerFalling:     ObjectPlayerStateFalling(Player);   break;
        case PlayerShooting:    ObjectPlayerStateShooting(Player);  break;
        case PlayerHurt:   ObjectPlayerStateHurt(Player); break;
        default:
            #if(DEBUG_MODE)  
                kprintf("Unimplemented playerstate %i", Player->State);
            #endif
        break;
    }
    const bool StateChanged = (Player->LastState != Player->State);

    ObjectPlayerUpdateSprite(Player, StateChanged);

    if(StateChanged)
    {
        Player->StateFrame = 0;
        #if(DEBUG_MODE)  
            kprintf("State change, %02X to %02X", Player->LastState, Player->State);
        #endif
    }
    else
    {
        ++Player->StateFrame;
    }

    // Physics
    const bool sens_feet_left  = col(x_left,y + 1);
    const bool sens_feet_mid   = col(x,y + 1);
    const bool sens_feet_right = col(x_right,y + 1);
    Player->OnFloor = (sens_feet_left | sens_feet_mid | sens_feet_right) > 0;

    // Just landed - pop out of floor
    if(Player->OnFloor)
    {
        if(!Player->OnfloorLast)
        {
            stuck = true;
            int its = 0;
            while(stuck && (its < 16))
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
            #if(DEBUG_MODE)
                if(its > 0)
                {
                    kprintf("Popped %i", its);
                }
            #endif
            Player->y = FIX32(y);
        }
    }
    else    // In air - apply gravity
    {
        Player->VelocityY = Player->VelocityY + gravity;
        //Player->VelocityX = fix16Mul(Player->VelocityX, friction);
    }

    if(Player->OnFloor || Player->CoyoteMode)
    {
        Player->VelocityY = min(Player->VelocityY, FIX16(0));  // Allow negative speed (i.e. jumping) but no falling
        // Apply friction. If ~ 0, stop.
        if((abs(fix16ToInt(Player->VelocityX)) <= 1) && (!Player->Controller.Pressed_Left && !Player->Controller.Pressed_Right))
        {
            Player->VelocityX = FIX16(0);
        }
        else
        {
            Player->VelocityX = fix16Mul(Player->VelocityX, friction);
        }
    }

    Player->OnfloorLast = Player->OnFloor;

    Player->Base.x = Player->x;
    Player->Base.y = Player->y;
    
    Player->LastState = Player->State;

    /*
    s32 x = fix32ToInt(Player->x);
    s32 y = fix32ToInt(Player->y);

    // Walking into walls sensor
    bool sens_top, sens_mid, sens_low;

    bool stuck = true;  // assume stuck
    int its = 0;
    bool moved = false;
    do
    {
        x = fix32ToInt(Player->x);
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
        if(Player->Controller.Pressed_Shoot)  // Jump
        {
            Player->VelocityY = JumpForce;  // Jump velocity
            Player->OnFloor = false;        // Detatch from floor
            Player->CoyoteFrames = 0;       // negate coyote mode immediately
            Player->JumpHold = 0;
        }
    }
    else
    {
        if(Player->Controller.Pressed_Shoot && Player->JumpHold != 0xFF)
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
        if((abs(fix16ToInt(Player->VelocityX)) <= 1) && (!Player->Controller.Pressed_Left && !Player->Controller.Pressed_Right))
        {
            Player->VelocityX = FIX16(0);
        }
        else
        {
            Player->VelocityX = fix16Mul(Player->VelocityX, friction);
        }

        // Gravity 
        Player->VelocityY = FIX16(0);
        if(Player->Controller.Pressed_Left)
        {
            Player->VelocityX = Player->VelocityX - acceleration;
            SPR_setAnim(Player->Base.spr, PlayerAnimWalk);
        }
        else if (Player->Controller.Pressed_Right)
        {
            Player->VelocityX = Player->VelocityX + acceleration;
            SPR_setAnim(Player->Base.spr, PlayerAnimWalk);
        }
    }
    else    // In air
    {
        Player->VelocityX = fix16Mul(Player->VelocityX, friction);
        if(Player->Controller.Pressed_Left)
        {
            Player->VelocityX = Player->VelocityX - air_acceleration;
        }
        else if (Player->Controller.Pressed_Right)
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
    */
    //SetCollisionRectangleAligned(&Player->Base.Collision, fix32ToInt(Player->Base.x), fix32ToInt(Player->Base.y)-2, 20, 40, BottomMiddle);
    //SetCollisionRectangle(&Player->Base.Collision, fix32ToInt(Player->Base.x) - 20, fix32ToInt(Player->Base.y) + 24, 20, 40);
}

void ObjectPlayerInit(void* object)
{
    ObjectPlayer* Player = (ObjectPlayer*)object;
    //
    ObjectPlayerInput(Player, 0x00);
    Player->State = PlayerFalling;
    Player->Controller.WalkDir = 1;
    Player->StateFrame = 0;
    //
    Player->Health = 100;
    Player->MaxHealth = 100;
    //
    Player->AnimationState = PlayerAnimStand;
    Player->AnimationTick = 0;
    // Physics
    Player->VelocityX = FIX16(0);
    Player->VelocityY = FIX16(0);
    Player->OnFloor = FALSE;
    Player->CoyoteFrames = 0;
    Player->CoyoteMode = false;
    // sprite
    Player->Base.spr = SPR_addSprite(&sprPlayer, 0,0, TILE_ATTR(PAL_PLAYER, 0,false,false));
    
    SPR_setVisibility(Player->Base.spr, AUTO_FAST);
    Player->Base.spriteOffset.x = -24;
    Player->Base.spriteOffset.y = -46;

    u16 pal[16];
    for(int i = 0; i < 16; ++i)
        pal[i] = sprPlayer.palette->data[i] / 2;

    memcpy(&GameContext.palette[PAL_PLAYER], sprPlayer.palette->data, 16 * 2);
    memcpy(&GameContext.paletteEffect[PAL_PLAYER], pal, 16 * 2);
}

void ObjectPlayerFree(void* object)
{
    ObjectPlayer* Player = (ObjectPlayer*)object;
    SPR_releaseSprite(Player->Base.spr);
}

void HurtPlayer(ObjectPlayer* Player)
{
    Player->State = PlayerHurt;
    Player->Health -= 1;
    Player->StateFrame = 0;
}