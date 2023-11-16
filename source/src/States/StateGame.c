#include "StateGame.h"
#include "State.h"

#include "resources.h"
#include "../defines.h"

#include "../Objects/Player.h"

bool Paused;
PlayerType Player;

u8 level1[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

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

    VDP_setPlaneSize(32,32, TRUE);
    PAL_setPalette(LEVEL_PALETTE, floortiles.palette->data,CPU);
    VDP_loadTileSet(floortiles.tileset,1,CPU);
    u8 x = 0;
    u8 y = 0;
    u8 t = 0;

    for(y = 0; y < 32; y++)
    {
        for (x = 0; x < 32; x++)
        {
            t = level1[y % 8][x % 8];
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(LEVEL_PALETTE, 0, FALSE, FALSE, t + 1), x, y);
        }
    }
}
void StateGame_End()
{
    PlayerFree(&Player);
    VDP_setBackgroundColor(0);
    VDP_clearPlane(BG_A, TRUE);
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
    SPR_update();
    
}

struct StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick,
};