#pragma once

#include <genesis.h>
#include "defines.h"

#include "Types/StateType.h"
#include "Types/StageBase.h"

/*
    What is going on and where it's happening.
*/

struct GameContextType
{
    //
    enum STATE_ID CurrentStateID;
    enum STATE_ID NextStateID;
    const StateType* CurrentState;
    // Region / speed stuff
    u8 Framerate;
    fix16 Speedup;
    //
    Map* CurrentMap;
    StageBase* CurrentStage;
    u8 CurrentStageID;
    u8 NextStageID;
    bool Paused;
    //
    u16 Frame;
    //
    Vect2D_s16 PlayerSpawn;     // Where to place player... Controlled by current level
};

extern struct GameContextType GameContext;

void GameContextInit();