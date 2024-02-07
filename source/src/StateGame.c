#include "StateGame.h"

#include <resources.h>

#include <genesis.h>

#include "GameContext.h"
#include "defines.h"

#include "Stages/Stages.h"

#include "ObjectManager.h"

#include "ObjectList.h"
#include "Objects/ObjectBase.h"
#include "Objects/ObjectCamera.h"
#include "Objects/ObjectPlayer.h"
#include "Objects/ObjectPickup.h"

struct Sprite* SpriteFreecam;
struct Sprite* SpritePaused;

struct ObjectPlayer* Player;

//struct MemoryPool ObjectMemory;

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
                    //ObjectPlayerInput(&Player1, 0x00);
                    //ObjectPlayerInput(&Player2, State);
                    ObjectCameraFreecam(GameContext.Camera, State);
                }
                else
                {
                    ObjectPlayerInput((struct ObjectPlayer*)(GameContext.Player), State);
                }
            }
            else
            {
                if(State & BUTTON_A)
                {
                    fix32 x = GameContext.Camera->Base.x + FIX32(320 / 2);
                    fix32 y = GameContext.Camera->Base.y + FIX32(224 / 2);

                    CreateObject(TypeObjectPickup, x, y);
                    /*
                    static int thing = 0;
                    switch(thing)
                    {
                        //case 0: CreateObject(TypeObjectPlayer, x, y); break;
                        case 1: CreateObject(TypeObjectPickup, x, y); break;
                        case 2: CreateObject(TypeEnemyBloober, x, y); break;
                        case 3: CreateObject(TypeEnemyFrogman, x, y); break;
                    }
                    thing = (thing + 1) % 3;
                    */
                }
                if(State & BUTTON_B)
                    GameContext.NextStateID = STATE_MENU;
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
}

// State entry points
void StateGame_Start()
{
    InitObjectManager();

    GameContext.CurrentStageID = 0xFF;
    GameContext.NextStageID = 1;    // Change to 0 once cutscene implemented

    JOY_setEventHandler(&StateGame_Joystick);

    StateGame_Reload();

    ObjectCameraInit(GameContext.Camera);
    //ObjectCameraSetTarget(GameContext.Camera, &Player1.Base);

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
    PAL_fadeOutAll(GameContext.Framerate, false);   // Fadeout animation

    SPR_end();          // Kill sprite engine
    EndObjectManager(); // Dump all game objects from memory

    GameContext.CurrentStageID = 0xFF;  // Invalidate the StageID so entry gets refired again on next StateGame start
}
void StateGame_Tick()
{
    s16 CameraX = fix32ToInt(GameContext.Camera->Base.x);
    s16 CameraY = fix32ToInt(GameContext.Camera->Base.y);

    // Stage logic
    if((GameContext.CurrentStageID != GameContext.NextStageID))   // If stage change triggered
    {
        SYS_disableInts();
        SPR_defragVRAM();
        SYS_enableInts();

        StateBootup = false;

        // Shut down last stage
        if(GameContext.CurrentStage != NULL)
        {
            PAL_fadeOutAll(GameContext.Framerate, false);
            GameContext.CurrentStage->Cleanup();    // Clear out stage memory/vram
            EndObjectManager();
            InitObjectManager();
        }
        // Find next
        GameContext.CurrentStage = GetStageData(GameContext.NextStageID);
        GameContext.CurrentStageID = GameContext.NextStageID;
        if(GameContext.CurrentStage != NULL)        // If stage loaded
        {
            SYS_disableInts();

            GameContext.CurrentStage->Init();       // Init incoming stage
            
            ObjectCameraSetStageSize(GameContext.Camera, GameContext.CurrentStage->Width, GameContext.CurrentStage->Height);
            GameContext.Paused = false;             // Ensure game is unpaused
            GameContext.StageFrame = 0;             // Reset stage timer    

            Player = CreateObject(TypeObjectPlayer, FIX32(GameContext.PlayerSpawn.x), FIX32(GameContext.PlayerSpawn.y));
            ObjectCameraSetTarget(GameContext.Camera, &Player->Base);
            GameContext.Player = &Player->Base;

            // Perform fade-in
            SYS_enableInts();
            TickObjects(); 
            
            GameContext.CurrentStage->Draw(
                fix32ToInt(GameContext.Camera->Base.x), 
                fix32ToInt(GameContext.Camera->Base.x)
            );

            VDP_waitDMACompletion();
            PAL_fadeIn(0, (4 * 16) - 1, GameContext.palette, GameContext.Framerate, FALSE);
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
            //GameContext.CurrentStage->Tick();
            TickObjects();
            if(!GameContext.Freecam)
            {
                ObjectCameraUpdate(GameContext.Camera);
            }
        }
    }

    // Collision test
    //if(CheckCollision(&Player1.Base.Collision, &Pickup.Base.Collision))
    //{
    //    Player1.Health -= 2;
    //}

    //int time = GameContext.StageFrame / GameContext.Framerate;
    //const char* buf;
    //const int len = sizeof(ObjectList) / sizeof(ObjectList[0]); 
    //for(int i = 0; i < len; ++i)
    //{
    //    buf = ObjectList[i].name;
    //    VDP_drawText(buf, 8, 8 + i);
    //}
    //char buf[20];
    //sprintf(buf, "MEM:%i", MEM_getFree());
    //VDP_drawText(buf, 8,8);
    //for(int i = 0; i < 10; ++i)
    //{
    //    struct ObjectBase* obj = GetObjectFromID(i);
    //    if(obj != NULL)
    //    {
    //        //strncpy(buf, ObjectList->name[obj->ActiveObjectID], 20);
    //        sprintf(buf, "%i:X:%li,Y:%li", obj->ObjectType, fix32ToInt(obj->x),fix32ToInt(obj->y));
    //        VDP_drawText(buf, 8,9+i);
    //    }
    //}
//
    // Paused or not, run map drawing logic
    if(GameContext.CurrentStage != NULL)
        GameContext.CurrentStage->Draw(CameraX, CameraY);

    ++GameContext.StageFrame;
    SPR_update();
}

const StateType StateGame = 
{
    StateGame_Start, StateGame_End, StateGame_Tick, 
};