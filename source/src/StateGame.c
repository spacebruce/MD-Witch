#include "StateGame.h"

#include <resources.h>
#include <genesis.h>

#include "GameContext.h"
#include "defines.h"

#include "Stages/Stages.h"

#include "ObjectPool.h"
#include "Objects/ObjectPlayer.h"
#include "Objects/ObjectPickup.h"

struct Sprite* SpritePaused;
struct Sprite* SpriteFreecam;

//struct MemoryPool ObjectMemory;

ObjectPlayer Player1;
ObjectPlayer Player2;
ObjectPickup Pickup;

//
bool LastPaused;
bool StateBootup;

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
                if(GameContext.Freecam)
                {
                    ObjectPlayerInput(&Player1, 0x00);
                    //ObjectPlayerInput(&Player2, State);
                    ObjectCameraFreecam(GameContext.Camera, State);
                }
                else
                {
                    ObjectPlayerInput(&Player1, State);
                }
            }
            else
            {
                if(State & BUTTON_C)
                    GameContext.Freecam = !GameContext.Freecam;
            }
        }
    }
}

// Reloads graphics we always want in memory
void StateGame_Reload() 
{
    SPR_init();
    SpritePaused = SPR_addSprite(&sprPaused, 112, 90, TILE_ATTR(PAL_PLAYER,0,false,false));
    SPR_setPriority(SpritePaused, true);
    SPR_setDepth(SpritePaused, 0);

    SpriteFreecam = SPR_addSprite(&sprFreecam, 16,16, TILE_ATTR(PAL_PLAYER, 0,false,false));
    SPR_setVisibility(SpriteFreecam, HIDDEN);
    
	PAL_setColors(0, (u16*) palette_black, 64, DMA);

    memcpy(&(GameContext.palette)[PAL_BACKGROUND], sprPlayer.palette->data, 16);
    memcpy(&(GameContext.palette)[PAL_PLAYER], sprPlayer.palette->data, 16);
    
	//PAL_fadeIn(0, (4 * 16) - 1, GameContext.palette, GameContext.Framerate, true);
}

// State entry points
void StateGame_Start()
{
    GameContext.CurrentStageID = 0xFF;
    GameContext.NextStageID = 1;    // Change to 0 once cutscene implemented

    JOY_setEventHandler(&StateGame_Joystick);

    StateGame_Reload();

    ObjectPlayerCreate(&Player1);
    ObjectPlayerCreate(&Player2);
    ObjectCameraInit(GameContext.Camera, &Player1.Base);

    ObjectPickupInit(&Pickup);
    ObjectSetPositionS32(&Pickup.Base, 96,96);

    StateBootup = true;

    GameContext.Paused = false;
    LastPaused = false;

    GameContext.Freecam = false;


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
            SYS_disableInts();
            GameContext.CurrentStage->Init();       // Init incoming stage
            SYS_enableInts();
            
            ObjectCameraSetStageSize(GameContext.Camera, GameContext.CurrentStage->Width, GameContext.CurrentStage->Height);
            GameContext.Paused = false;             // Ensure game is unpaused
            GameContext.StageFrame = 0;             // Reset stage timer    
            GameContext.Player = &Player1.Base;
            ObjectSetPositionS32(&Player1.Base, GameContext.PlayerSpawn.x, GameContext.PlayerSpawn.y); 
            ObjectSetPositionS32(&Player2.Base, GameContext.PlayerSpawn.x, GameContext.PlayerSpawn.y); 
        }
    }
    
    if(GameContext.Freecam)
    {
        SPR_setVisibility(SpriteFreecam, VISIBLE);
        SPR_setDepth(SpriteFreecam, 0);
    }
    else
    {
        SPR_setVisibility(SpriteFreecam, HIDDEN);
    }

    // Catch Paused/Unpaused state change
    if((LastPaused != GameContext.Paused) || (GameContext.StageFrame == 0))
    {
        if(GameContext.Paused)
        {
            SPR_setVisibility(SpritePaused, VISIBLE);
        }
        else
        {
            SPR_setVisibility(SpritePaused, HIDDEN);
        }
        LastPaused = GameContext.Paused;
    }

    if(!GameContext.Paused)
    {
        if(GameContext.CurrentStage != NULL)
        {
            GameContext.CurrentStage->Tick();
            ObjectPlayerUpdate(&Player1);
            ObjectPlayerUpdate(&Player2);
            ObjectPickupUpdate(&Pickup);
            if(!GameContext.Freecam)
            {
                ObjectCameraUpdate(GameContext.Camera);
            }
        }
    }
    s16 CameraX = fix32ToInt(GameContext.Camera->Base.x);
    s16 CameraY = fix32ToInt(GameContext.Camera->Base.y);

    // char buffer[20];
    // sprintf(buffer, "X:%i\nY:%i", fix32ToInt(GameContext.Player->x), fix32ToInt(GameContext.Player->y));
    // VDP_drawText(buffer, 10,10);
    
    if(GameContext.MapA != NULL)
        MAP_scrollTo(GameContext.MapA, CameraX, CameraY);
    if(GameContext.MapB != NULL)
        MAP_scrollTo(GameContext.MapB, (CameraX >> 2), (CameraY >> 2));

    //VDP_setHorizontalScroll(BG_B, (-CameraX >> 1));
    //VDP_setVerticalScroll(BG_B, -CameraY);

    // update all sprites
    //SPR_setPosition(SpritePlayer, Player.Base.x - 24, Player.Base.y - 48);
    ObjectUpdateSprite(&Player1.Base, GameContext.Camera->Base.x, GameContext.Camera->Base.y);
    ObjectUpdateSprite(&Player2.Base, GameContext.Camera->Base.x, GameContext.Camera->Base.y);
    ObjectUpdateSprite(&Pickup.Base, GameContext.Camera->Base.x, GameContext.Camera->Base.y);

    // Collision test
    if(CheckCollision(&Player1.Base.Collision, &Pickup.Base.Collision))
    {
        Player1.Health -= 2;
    }

    //int time = GameContext.StageFrame / GameContext.Framerate;
    char buf[16];
    sprintf(buf, "%i/%i", Player1.Health,Player1.MaxHealth);
    VDP_drawText(buf, 4,4);
    VDP_drawText(buf, 8,8);
    VDP_drawText(buf, 16,16);
    
    // Paused or not, run map drawing logic
    if(GameContext.CurrentStage != NULL)
        GameContext.CurrentStage->Draw();

    ++GameContext.StageFrame;
    SPR_update();
}

const StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick, 
};