#include "StateGame.h"
#include "State.h"

#include "../Objects/Player.h"

bool Paused;
PlayerType Player;

void StateGame_Joystick(u16 Joy, u16 Changed, u16 State)
{
    if(Changed & State & BUTTON_START)
    {
        Paused = !Paused;
    }
    if(!Paused)
    {
        PlayerInput(&Player, Joy, Changed, State);
    }
    else
    {
        // Redirect to pause menu?
    }
}
void StateGame_Start()
{
    JOY_setEventHandler(&StateGame_Joystick);
    
    SPR_init();
    PlayerInit(&Player);
}
void StateGame_End()
{
    PlayerFree(&Player);
    VDP_setBackgroundColor(0);
    VDP_clearPlane(BG_B, TRUE);
}

s8 pal = 0;
void StateGame_Tick()
{
    if(!Paused)
    {
        PlayerUpdate(&Player);
        
        pal = (pal + 1) % 64;
        if(pal == 0)
            STATE_NEXT = STATE_MENU;
    }

    PlayerDraw(&Player);
    VDP_setBackgroundColor(pal);
    VDP_drawTextBG(BG_B, "This is what a game looks like", 0,20);

    if(Paused)
    {
        VDP_drawTextBG(BG_B, "Paused!", 10,0);
        return;
    }
    
}

struct StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick,
};