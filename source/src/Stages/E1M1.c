#include "E1M1.h"

#include "../GameContext.h"
#include <genesis.h>
#include <resources.h>

void E1M1_Init()
{
    GameContext.PlayerSpawn = (Vect2D_s16){ 60, 160 };
}
void E1M1_Tick()
{
    VDP_setVerticalScroll(BG_A,-128);
    VDP_setVerticalScroll(BG_B,-128);
    
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