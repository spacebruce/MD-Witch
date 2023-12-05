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
                ObjectPlayerInput(&Player, Changed, State);
            }
        }
    }
}

// Reloads graphics we always want in memory
void StateGame_Reload() 
{
    SPR_init();
    SpritePaused = SPR_addSprite(&sprPaused, 112, 90, TILE_ATTR(PAL_PLAYER,0,false,true));
    SPR_setPriority(SpritePaused, true);
    SpritePlayer = SPR_addSprite(&sprPlayer, 32,32, TILE_ATTR(PAL_PLAYER, 0,false,false));
    PAL_setPalette(PAL0, sprPlayer.palette->data, DMA);
    PAL_setPalette(PAL_PLAYER, sprPlayer.palette->data, DMA);   // Many static objects share player palette
    VDP_setTextPalette(PAL_PLAYER);
}

// State entry points
void StateGame_Start()
{
    GameContext.CurrentStageID = 0xFF;
    GameContext.NextStageID = 1;    // Change to 0 once cutscene implemented

    JOY_setEventHandler(&StateGame_Joystick);

    ObjectPlayerCreate(&Player);
    ObjectCameraInit(GameContext.Camera, &Player.Base);

    GameContext.Paused = false;
    LastPaused = false;

    StateGame_Reload();

	VDP_setPlaneSize(64,64, true);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

	//VDP_fillTileMapRect(BG_B,0x0C,0,0,64,39);

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
    GameContext.CurrentStageID = 0xFF;  // Invalidate the StageID so entry gets refired again on next StateGame start
}
void StateGame_Tick()
{
    // Stage logic
    if((GameContext.CurrentStageID != GameContext.NextStageID))   // If stage change triggered
    {
        StateBootup = false;
        if(GameContext.CurrentStage != NULL)
        {
            GameContext.CurrentStage->Cleanup();    // Clear out stage memory/vram
        }
        GameContext.CurrentStage = GetStageData(GameContext.NextStageID);
        GameContext.CurrentStageID = GameContext.NextStageID;
        if(GameContext.CurrentStage != NULL)        // If stage loaded
        {
            GameContext.CurrentStage->Init();       // Init incoming stage
            GameContext.Paused = false;             // Ensure game is unpaused
            GameContext.StageFrame = 0;             // Reset stage timer    
            GameContext.Player = &Player.Base;
            Player.Base.x = intToFix32(GameContext.PlayerSpawn.x);  // Move player to spawn location
            Player.Base.y = intToFix32(GameContext.PlayerSpawn.y);
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
            ObjectPlayerUpdate(&Player);
            ObjectCameraUpdate(GameContext.Camera);
        }
    }
    ++GameContext.StageFrame;
    s16 CameraX = GameContext.Camera->Base.x;
    s16 CameraY = GameContext.Camera->Base.y;

    VDP_setHorizontalScroll(BG_A, -GameContext.Camera->Base.x);
    VDP_setVerticalScroll(BG_A, 0);
    
    VDP_setHorizontalScroll(BG_B, -(GameContext.Camera->Base.x >> 2));
    VDP_setVerticalScroll(BG_B, 0);

    // update all sprites
    SPR_setPosition(SpritePlayer, (Player.Base.x - 24) - CameraX, (Player.Base.y - 48) - CameraY);

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