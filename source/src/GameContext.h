#pragma once

#include <genesis.h>
#include "defines.h"

#include "Types/StateType.h"
#include "Types/StageBase.h"

#include "Objects/ObjectCamera.h"

/*
    What is going on and where it's happening.
*/

struct GameContextType
{
    //
    enum STATE_ID CurrentStateID;
    enum STATE_ID NextStateID;
    const StateType* CurrentState;
    // Game globals
    s16 BackgroundScroll[240];
    u16 palette[4][16];
    u16 paletteEffect[4][16];
    // Region / speed stuff
    u8 Framerate;
    fix16 Speedup;
    //
    Map* MapA;
    Map* MapB;
    //
    Map* CurrentMap;
    const StageBase* CurrentStage;
    u8 CurrentStageID;
    u8 NextStageID;
    u16 StageFrame;
    fix32 StageTime;
    bool Paused;
    //  Debug switches
    bool Freecam;   // debug
    //
    u16 Frame;
    //
    ObjectCamera* Camera;       // Active game camera
    struct ObjectBase* Player;         // Make it easier to track player
    Vect2D_s32 PlayerSpawn;     // Where to place player... Controlled by current level
};

extern struct GameContextType GameContext;

void GameContextInit();
void GameContextTick();