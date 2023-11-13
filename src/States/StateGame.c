#include "StateGame.h"
#include "State.h"

#include "../Objects/Player.h"
Player player;

void StateGame_Joystick(u16 Joy, u16 Changed, u16 State)
{

}
void StateGame_Start()
{
    JOY_setEventHandler(&StateGame_Joystick);
}
void StateGame_End()
{
    VDP_clearText(0,20,40);
    VDP_setBackgroundColor(0);
}

s8 pal = 0;
void StateGame_Tick()
{
    VDP_setBackgroundColor(pal);
    VDP_drawText("This is what a game looks like", 0,20);
    pal = (pal + 1) % 64;
    if(pal == 0)
        STATE_NEXT = STATE_MENU;
}

struct StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick,
};