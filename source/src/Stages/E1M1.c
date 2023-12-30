#include "E1M1.h"

#include "../GameContext.h"
#include <genesis.h>

#include <maps.h>
#include <resources.h>

#include "../Types/Collision.h"

const int e1m1_tile_xsize = 4 * 8;
const int e1m1_tile_ysize = 4 * 8;


// Tile IDs
u16 tile_id;
u16 bg_tile_id;

const int e1m1_width = 16;
const int e1m1_height = 16;

uint8_t e1m1_collisions[16][16] = 
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
    GameContext.PlayerSpawn.x = 0;
    GameContext.PlayerSpawn.y = 128;

	tile_id = TILE_USER_INDEX;
	bg_tile_id = tile_id;
	VDP_loadTileSet(&ts_stage_01, bg_tile_id, DMA);
	tile_id += ts_stage_01.numTile;
    
	GameContext.MapA = MAP_create(&map_stage_01, BG_A, bg_tile_id);
	GameContext.MapB = MAP_create(&map_stage_01_bg, BG_B, bg_tile_id);
    
	DMA_setBufferSize(9000);

	memcpy(&GameContext.palette[0], pal_stage_01a.data, 16 * 2);
	memcpy(&GameContext.palette[16], pal_stage_01b.data, 16 * 2);
	//memcpy(&GameContext.palette[32], pal_player.data, 16 * 2);
	memcpy(&GameContext.palette[48], (u16*) palette_black, 16 * 2);

	// Fade In
	PAL_fadeIn(0, (4 * 16) - 1, GameContext.palette, 60, FALSE);
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
	512,512,
};