#include "E1M1.h"

#include "../GameContext.h"
#include <genesis.h>
#include <resources.h>

#include "../Types/Collision.h"

const int e1m1_tile_xsize = 6 * 8;
const int e1m1_tile_ysize = 4 * 8;

uint8_t e1m1_collisions[8][16] = 
{
    { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 2, 2, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, },
    { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, },
    { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, },
};

u8 E1M1_Collide(const s16 x, const s16 y)
{
    u16 tx = (((u16)(x)) / e1m1_tile_xsize) % 16;
    u16 ty = (((u16)(y)) / e1m1_tile_ysize) % 8;
    if(e1m1_collisions[ty][tx] > 0)
    {
        MapCollision.TileX = tx;
        MapCollision.TileY = ty;
        return true;
    }
    return false;
}

void E1M1_Init()
{
    GameContext.PlayerSpawn.x = 0;
    GameContext.PlayerSpawn.y = 50;

    VDP_setTextPlane(BG_A);
    for(int x = 0; x < 16; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(e1m1_collisions[y][x] == 1)
            {
                for(int sx = 0; sx < 6; ++sx)
                {
                    for(int sy = 0; sy < 4; ++ sy)
                    {
                        VDP_drawText("X", (x * 6) + sx , (y * 4) + sy);
                    }
                }
            }
            if(e1m1_collisions[y][x] == 2)
            {
                for(int sx = 0; sx < 6; ++sx)
                {
                    for(int sy = 0; sy < 2; ++ sy)
                    {
                        VDP_drawText("J", (x * 6) + sx , (y * 4) + sy);
                    }
                }
            }
        }
    }
    DMA_waitCompletion();

    VDP_setTextPlane(BG_B);

    for(int x = 0; x < 64; x += strlen(GameContext.CurrentStage->Name))
    {
        for(int i = 0; i < 32; ++i)
        {
            //VDP_drawText(GameContext.CurrentStage->Name,x,i);
        }
    }

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
    E1M1_Collide,
    E1M1_Draw,
    E1M1_End,
};