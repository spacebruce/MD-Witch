#pragma once

#include <genesis.h>

/*
    Works much the same way as the main game state. 
    Levels define a struct containing callbacks for init/tick/draw code, this way they are easily expandable and can have custom logic.
*/
typedef void (*StageFunctionVoid)();
typedef u8 (*StageFunctionCollision)(s16, s16);

typedef struct 
{
    const char* Name;
    StageFunctionVoid Init;
    StageFunctionVoid Tick;     // Tick and draw are independant so pause doesn't fuck drawing up
    StageFunctionCollision Collision;
    StageFunctionVoid Draw;     
    StageFunctionVoid Cleanup;
    u16 Width;
    u16 Height;
} StageBase;