#include "E1M1.h"

#include "../GameContext.h"
#include <genesis.h>
#include <resources.h>

void E1M1_Init()
{
    GameContext.PlayerSpawn.x = 100;
    GameContext.PlayerSpawn.y = 50;
    
}
void E1M1_Tick()
{
    for(int i = 0; i < 32; ++i)
        VDP_drawText(GameContext.CurrentStage->Name,0,i);
    DMA_waitCompletion();
    
    if(GameContext.Player->x > 1000)
        GameContext.NextStateID = STATE_MENU;
}
void E1M1_End()
{
}
void E1M1_Draw()
{
}

const StageBase E1M1 = 
{
    "E1M1",
    E1M1_Init,
    E1M1_Tick,
    E1M1_Draw,
    E1M1_End,
};