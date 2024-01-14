#include "E1M1.h"

#include "../GameContext.h"
#include <genesis.h>

#include <maps.h>
#include <resources.h>

#include "../Types/Collision.h"

const int e1m1_tile_xsize = 4 * 8;
const int e1m1_tile_ysize = 4 * 8;

/*
	hint test 
*/
static vu16  lineDisplay   = 0;             // line position on display screen
void VBlankHandler()
{
    lineDisplay = 0;
}
HINTERRUPT_CALLBACK HIntHandler()
{
    VDP_setHorizontalScroll(BG_B, lineDisplay);
    lineDisplay++;
}

// Tile IDs
u16 tile_id;
u16 bg_tile_id;

static const int e1m1_width = 16;
static const int e1m1_height = 16;

const uint8_t e1m1_collisions[16][16] = 
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
	{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
	{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, },
	{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
};

u8 E1M1_Collide(const s16 x, const s16 y)
{
    u16 tx = (((u16)(x)) / e1m1_tile_xsize) % e1m1_width;
    u16 ty = (((u16)(y)) / e1m1_tile_ysize) % e1m1_height;
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
    GameContext.PlayerSpawn.x = 128;
    GameContext.PlayerSpawn.y = 90;

	//SYS_setHIntCallback(HIntHandler);
	//SYS_setVBlankCallback(VBlankHandler);
	//VDP_setHIntCounter(0);
	//VDP_setHInterrupt(1);

	tile_id = TILE_USER_INDEX;
	bg_tile_id = tile_id;
	VDP_loadTileSet(&ts_stage_01, bg_tile_id, DMA);
	tile_id += ts_stage_01.numTile;
    
	GameContext.MapA = MAP_create(&map_stage_01, BG_A, bg_tile_id);
	GameContext.MapB = MAP_create(&map_stage_01_bg, BG_B, bg_tile_id);

	DMA_setBufferSize(9000);

	PAL_setPalette(PAL_BACKGROUND, pal_stage_01a.data, DMA);
	PAL_setPalette(PAL_TILES, pal_stage_01b.data, DMA);

	//memcpy(&GameContext.palette[PAL_BACKGROUND * 16], pal_stage_01a.data, 16 * 2);
	//memcpy(&GameContext.palette[PAL_TILES * 16], pal_stage_01b.data, 16 * 2);
	//memcpy(&GameContext.palette[PAL_PLAYER * 16], sprPlayer.palette->data, 16 * 2);
	//memcpy(&GameContext.palette[PAL_ENEMIES * 16], (u16*) palette_black, 16 * 2);
//
	//// Fade In
	//PAL_fadeIn(0, (4 * 16) - 1, GameContext.palette, GameContext.Framerate, FALSE);
}
void E1M1_Tick()
{

}
void E1M1_End()
{
	MAP_release(GameContext.MapA);
	MAP_release(GameContext.MapB);
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
	512,512,
};