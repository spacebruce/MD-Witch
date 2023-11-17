#include "StateGame.h"
#include "State.h"

#include <resources.h>
#include "../defines.h"
#include "../GameState.h"
#include "../VDPState.h"
#include "../Objects/Player.h"
#include "../Stages/Stages.h"

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
        if(Changed & State & BUTTON_A)
            Player.Lives = -1;
        // Redirect to pause menu?
    }
}
void StateGame_Start()
{
    JOY_setEventHandler(&StateGame_Joystick);
    
    SPR_init();
    PlayerInit(&Player);

    GAME_LEVELPTR = GetLevelFromID(1);
    if(GAME_LEVELPTR != NULL)
    {
        GAME_LEVELPTR->Init();
    }
    else
    {
        STATE_NEXT = STATE_MENU;    //Maybe go to an error handler screen?
    }
}
void StateGame_End()
{
    PlayerFree(&Player);
    VDP_setBackgroundColor(0);
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
}

void StateGame_EndTick()
{
}
void StateGame_Tick()
{
    if(GAME_LEVELPTR != NULL)
    {
        GAME_LEVELPTR->Tick();
        GAME_LEVELPTR->Draw();
    }

    if(!Paused)
    {
        PlayerUpdate(&Player);
        if(Player.Lives < 0)
            STATE_NEXT = STATE_MENU;
    }

    PlayerDraw(&Player);
    VDP_drawTextBG(BG_A, GAME_LEVELPTR->Name, 0,20);

    SPR_update();
    if(Paused)
    {
        VDP_drawTextBG(BG_A, "Paused!", 10,0);
        return;
    }
}

const StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick, StateGame_EndTick
};