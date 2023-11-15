#include "StateGame.h"
#include "State.h"

#include "resources.h"

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

    VDP_loadTileSet(bgtile.tileset,1,DMA);
    VDP_fillTileMapRect(BG_B, 1,0,0,40,30);
    PAL_setPalette(PAL0, bgtile.palette->data, DMA);
}
void StateGame_End()
{
    PlayerFree(&Player);
    VDP_setBackgroundColor(0);
    VDP_clearPlane(BG_B, TRUE);
}

void StateGame_Tick()
{
    if(!Paused)
    {
        PlayerUpdate(&Player);
        if(Player.Lives < 0)
            STATE_NEXT = STATE_MENU;
    }

    PlayerDraw(&Player);
    VDP_drawTextBG(BG_A, "This is what a game looks like", 0,20);

    if(Paused)
    {
        VDP_drawTextBG(BG_A, "Paused!", 10,0);
        return;
    }
    
}

struct StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick,
};