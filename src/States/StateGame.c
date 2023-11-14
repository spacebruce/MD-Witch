#include "StateGame.h"
#include "State.h"

#include "../Objects/Player.h"

bool Paused;
Player player;

void StateGame_Joystick(u16 Joy, u16 Changed, u16 State)
{
    if(Changed & State & BUTTON_START)
        Paused = !Paused;
}
void StateGame_Start()
{
    JOY_setEventHandler(&StateGame_Joystick);
}
void StateGame_End()
{
    VDP_setBackgroundColor(0);
    VDP_clearPlane(BG_B, TRUE);
}

s8 pal = 0;
void StateGame_Tick()
{
    VDP_setBackgroundColor(pal);
    VDP_drawTextBG(BG_B, "This is what a game looks like", 0,20);

    if(Paused)
    {
        VDP_drawTextBG(BG_B, "Paused!", 10,0);
        return;
    }
    
    pal = (pal + 1) % 64;
    if(pal == 0)
        STATE_NEXT = STATE_MENU;
}

struct StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick,
};