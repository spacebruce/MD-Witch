#pragma once

/*
    Works much the same way as the main game state. 
    Levels define a struct containing callbacks for init/tick/draw code, this way they are easily expandable and can have custom logic.
*/

typedef void (*StageFunction)();

typedef struct 
{
    const char* Name;
    StageFunction Init;
    StageFunction Tick;
    StageFunction Draw;
} StageBase;