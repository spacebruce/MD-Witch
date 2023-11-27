#include "StateGame.h"

#include <resources.h>
#include <genesis.h>

#include "GameContext.h"
#include "defines.h"

#include "Stages/Stages.h"

struct Sprite* SpritePaused;
struct Sprite* SpritePlayer;

//
bool LastPaused;

// Joystick
void StateGame_Joystick(u16 Joy, u16 Changed, u16 State)
{
    if(Joy == JOY_1)
    {
        if(State & BUTTON_START)
            GameContext.Paused = !GameContext.Paused;
    }
}

// Reloads graphics we always want in memory
void StateGame_Reload() 
{
    SPR_init();
    SpritePaused = SPR_addSprite(&sprPaused, 0, 0, TILE_ATTR(PAL_PLAYER,0,false,false));
    SpritePlayer = SPR_addSprite(&sprPlayer, 32,32, TILE_ATTR(PAL_PLAYER, 0,false,false));
    PAL_setPalette(PAL_PLAYER, sprPlayer.palette->data, DMA);   // Many static objects share player palette
}

// State entry points
void StateGame_Start()
{
    JOY_setEventHandler(&StateGame_Joystick);

    GameContext.Paused = false;
    LastPaused = false;

    StateGame_Reload();

	VDP_setPlaneSize(64,64, true);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

	VDP_fillTileMapRect(BG_B,0x0C,0,0,64,39);

    VDP_setTextPlane(BG_A);

}
void StateGame_End()
{
    //PAL_fadeOutAll(GameContext.Framerate, false);
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_clearSprites();
    SPR_end();
}
void StateGame_Tick()
{
    // Catch Paused/Unpaused state change
    if(LastPaused != GameContext.Paused)
    {
        if(GameContext.Paused)
        {
            SPR_setVisibility(SpritePaused, true);
        }
        else
        {
            SPR_setVisibility(SpritePaused, false);
        }
        LastPaused = GameContext.Paused;
    }

    // Stage logic
    if(!GameContext.Paused)
    {
        // Run stage specific code
        if(GameContext.CurrentStageID != GameContext.NextStageID)
        {
            if(GameContext.CurrentStage != NULL)
            {
                GameContext.CurrentStage->Cleanup();    // Clear out stage memory/vram
            }
            GameContext.CurrentStage = GetStageData(GameContext.NextStageID);
            GameContext.CurrentStageID = GameContext.NextStageID;
            if(GameContext.CurrentStage == NULL)
            {
                GameContext.CurrentStage->Init();       // Init incoming stage
            }
        }
        else
        {
            if(GameContext.CurrentStage != NULL)
            {
                GameContext.CurrentStage->Tick();
            }
        }
    }
        
    // Paused or not, run map drawing logic
    if(GameContext.CurrentStage != NULL)
        GameContext.CurrentStage->Draw();

    SPR_update();
}

const StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick, 
};