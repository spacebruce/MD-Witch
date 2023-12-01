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