#include "StateGame.h"

#include <resources.h>
#include <genesis.h>

#include "GameContext.h"
#include "defines.h"

#include "Objects/ObjectPlayer.h"
#include "Stages/Stages.h"

struct Sprite* SpritePaused;
struct Sprite* SpritePlayer;

ObjectPlayer Player;

//
bool LastPaused;

// Joystick
void StateGame_Joystick(u16 Joy, u16 Changed, u16 State)
{
    if(Joy == JOY_1)
    {
        if(State & BUTTON_START)
        {
            GameContext.Paused = !GameContext.Paused;
        }
        else
        {
            if(!GameContext.Paused)
            {
                inputObjectPlayer(&Player, Changed, State);
            }
        }
        
    }
}

// Reloads graphics we always want in memory
void StateGame_Reload() 
{
    SPR_init();
    SpritePaused = SPR_addSprite(&sprPaused, 112, 90, TILE_ATTR(PAL_PLAYER,0,false,false));
    SpritePlayer = SPR_addSprite(&sprPlayer, 32,32, TILE_ATTR(PAL_PLAYER, 0,false,false));
    PAL_setPalette(PAL_PLAYER, sprPlayer.palette->data, DMA);   // Many static objects share player palette
    VDP_setTextPalette(PAL_PLAYER);
}

// State entry points
void StateGame_Start()
{
    GameContext.NextStageID = 1;

    JOY_setEventHandler(&StateGame_Joystick);

    createObjectPlayer(&Player);

    GameContext.Paused = false;
    LastPaused = false;

    StateGame_Reload();

	VDP_setPlaneSize(64,64, true);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

	VDP_fillTileMapRect(BG_B,0x0C,0,0,64,39);

    VDP_setTextPlane(BG_A);
    VDP_setTextPalette(PAL_PLAYER);

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
    // Stage logic
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
            VDP_setTextPalette(PAL_PLAYER);
            VDP_setTextPlane(BG_A);
            GameContext.StageFrame = 0;
            Player.Base.x = GameContext.PlayerSpawn.x;
            Player.Base.y = GameContext.PlayerSpawn.y;
        }
    }

    // Catch Paused/Unpaused state change
    //if((LastPaused != GameContext.Paused) || (GameContext.StageFrame == 0))
    {
        if(GameContext.Paused)
        {
            SPR_setVisibility(SpritePaused, false);
        }
        else
        {
            SPR_setVisibility(SpritePaused, true);
        }
        LastPaused = GameContext.Paused;
    }

    if(!GameContext.Paused)
    {
        if(GameContext.CurrentStage != NULL)
        {
            GameContext.CurrentStage->Tick();
            updateObjectPlayer(&Player);
            SPR_setPosition(SpritePlayer, fix32ToInt(Player.Base.x) - 24, fix32ToInt(Player.Base.y) - 48);
            ++GameContext.StageFrame;
        }
    }

    //int time = GameContext.StageFrame / GameContext.Framerate;
    //char buf[16];
    //sprintf(buf, "frame:%i\ntime:%i", GameContext.StageFrame, time);
    //VDP_drawText(buf, 1,32);
        
    // Paused or not, run map drawing logic
    if(GameContext.CurrentStage != NULL)
        GameContext.CurrentStage->Draw();

    SPR_update();
}

const StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick, 
};