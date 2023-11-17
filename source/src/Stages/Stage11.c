#include "Stage11.h"

#include <genesis.h>
#include <resources.h>

#include "../defines.h"
#include "../VDPState.h"
#include "StageData.h"

u8 level1[8][8] = 
{
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

void Stage11_Init()
{
    //Stage11_DataInit(&StageData.Stage11);   // Init global vars for current stage
    VDP_setPlaneSize(32,32, TRUE);
    VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);

    PAL_setPalette(LEVEL_PALETTE, floortiles.palette->data,CPU);
    VDP_loadTileSet(floortiles.tileset,1,CPU);

    u8 x = 0;
    u8 y = 0;
    u8 t = 0;
    for(y = 0; y < 32; y++)
    {
        for (x = 0; x < 32; x++)
        {
            t = level1[y % 8][x % 8];
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(LEVEL_PALETTE, 0, FALSE, FALSE, t + 1), x, y);
        }
    }
}
void Stage11_Tick()
{
    // Scroll BG
    int i;
    for(i = 0; i < NUM_LINES; ++i)
        BGA_Scroll[i] = 0;//frame;// + i;
    
}
void Stage11_Draw()
{

}

const StageBase Stage11 = 
{
    Stage11_Init,
    Stage11_Tick,
    Stage11_Draw,
};