#include "E1M1.h"

#include "../GameContext.h"
#include <genesis.h>

#include <maps.h>
#include <resources.h>

#include "../Types/MapCollision.h"

const int e1m1_tile_xsize = 4 * 8;
const int e1m1_tile_ysize = 4 * 8;

/*
	hint test 
*/
static vs16 WaterLine = 0;            
static vs16 CameraPositionX = 0;
static vs16 CameraPositionY = 0;
static vs16 WaterY = 600;
void VBlankHandler()
{
	WaterLine = (WaterY - CameraPositionY);
	if(WaterLine > 240)		// Waterline off-screen
	{	
		VDP_setHInterrupt(0);	//Disable effect
		PAL_setColors(0, &GameContext.palette, 64, DMA);	// load normal stage palette
	}
	else if(WaterLine > 0)	// Camera above waterline, on-screen
	{
		VDP_setHInterrupt(1);	// Enable effect
		PAL_setColors(0, &GameContext.palette, 64, DMA);	// load normal stage palette
	}
	else				// Below - 
	{	
		VDP_setHInterrupt(0);	// Disable int
		PAL_setColors(0, &GameContext.paletteEffect, 64, DMA);	// Load effect palette
	}
}

HINTERRUPT_CALLBACK HIntHandler()
{
	--WaterLine;
	if(WaterLine == 0)
	{
		PAL_setColors(0,&GameContext.paletteEffect, 64, DMA);
	}
}

// Tile IDs
u16 tile_id;
u16 bg_tile_id;

static const int e1m1_width = 32;
static const int e1m1_height = 32;

const u16 e1m1_collisions[1024] =	
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 
};

u8 E1M1_Collide(const s16 x, const s16 y)
{
    const u16 tx = (((u16)(x)) / e1m1_tile_xsize);
    const u16 ty = (((u16)(y)) / e1m1_tile_ysize);

    if(e1m1_collisions[tx + (ty * e1m1_width)] > 0)
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

	SYS_setHIntCallback(HIntHandler);
	SYS_setVBlankCallback(VBlankHandler);
	VDP_setHIntCounter(0);
	VDP_setHInterrupt(1);

	VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);

	tile_id = TILE_USER_INDEX;

	VDP_loadTileSet(&bg_e1m1.tileset, tile_id, DMA);
	uint16_t bg_index = tile_id;
	tile_id += bg_e1m1.tileset->numTile;

	uint16_t tile_index = tile_id;
	VDP_loadTileSet(&ts_stage_01, tile_index, DMA);
	tile_id += ts_stage_01.numTile;

	//VDP_drawImage(BG_B, &bg_e1m1, 128,0);
	//VDP_drawImage(BG_B, &bg_e1m1, 256,0);
	//VDP_drawImage(BG_B, &bg_e1m1, 384,0);
    
	GameContext.MapA = MAP_create(&map_stage_01, BG_A, tile_index);
	//GameContext.MapB = MAP_create(&map_stage_01_bg, BG_B, bg_tile_id);

	// buffer the palettes to memory, these get sent to hardware on vsync
	memcpy(&GameContext.palette[PAL_BACKGROUND],  bg_e1m1.palette->data, 16 * 2);
	memcpy(&GameContext.palette[PAL_TILES],  pal_stage_01b.data, 16 * 2);
	memcpy(&GameContext.paletteEffect[PAL_BACKGROUND],  pal_stage_01b.data, 16 * 2);
	memcpy(&GameContext.paletteEffect[PAL_TILES],  pal_stage_01b.data, 16 * 2);

	VDP_drawImageEx(BG_B, &bg_e1m1, bg_index, 0,0, false, true);
//
	//// Fade In
}
void E1M1_Tick()
{
}
void E1M1_End()
{
	MAP_release(GameContext.MapA);
	MAP_release(GameContext.MapB);
}
void E1M1_Draw(s16 CameraX, s16 CameraY)
{
	CameraPositionX = CameraX;
	CameraPositionY = CameraY;

    if(GameContext.MapA != NULL)
        MAP_scrollTo(GameContext.MapA, CameraX, CameraY);

    //if(GameContext.MapB != NULL)
    //    MAP_scrollTo(GameContext.MapB, (CameraX >> 2), (CameraY >> 2));
}

const StageBase E1M1 = 
{
    "E1M1",
    E1M1_Init,
    E1M1_Tick,
    E1M1_Collide,
    E1M1_Draw,
    E1M1_End,
	1024,1024,
};